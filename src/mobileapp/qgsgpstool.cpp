/***************************************************************************
    qgsgpstool.cpp  -  Gps connection
     --------------------------------------
    Date                 : 08-Aug-2012
    Copyright            : (C) 2012 by Ramon Carrillo
    Email                : racarrillo91 at gmail.com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qgsgpstool.h"

#include "qgsgpsmarker.h"
#include "qgsnmeaconnection.h"
#include "qgsgpsconnectionregistry.h"
#include "qgsgpsdetector.h"
#include "info.h"
#include "qgscoordinatetransform.h"
#include "qgsmapcanvas.h"
#include "qgsmaprenderer.h"
#include <QSettings>
#include "qgsrubberband.h"
#include <QMessageBox>
#include "qgsapplication.h"
#include "qgsfeatureaction.h"
#include "qgsvectorlayer.h"

QgsGPSTool::QgsGPSTool(QWidget *parent, QgsMapCanvas *canvas)
    : mView( parent )
    , mNmea( 0 )
    , mpCanvas( canvas )
    , mpMapMarker( 0 )
    , mRecenter( true )
    , mAutoAddVertices( false )
    , mAutoCommit( false )
{
  mWgs84CRS.createFromOgcWmsCrs( "EPSG:4326" );
}

QgsGPSTool::~QgsGPSTool()
{
  if ( mNmea )
  {
    disconnectGps();
  }

  if ( mpMapMarker )
    delete mpMapMarker;
}

void QgsGPSTool::connectGps()
{
  // If it's connected then recenter
  if ( mNmea ) {
    mRecenter = true;
  }

  mLastGpsPosition = QgsPoint( 0.0, 0.0 );

  QString port;

  port = QString( "internalGPS" );

  QgsGPSDetector *detector = new QgsGPSDetector( port );
  connect( detector, SIGNAL( detected( QgsGPSConnection * ) ), this, SLOT( connected( QgsGPSConnection * ) ) );
  connect( detector, SIGNAL( detectionFailed() ), this, SLOT( timedout() ) );
  detector->advance();   // start the detection process
}

void QgsGPSTool::timedout()
{
}

void QgsGPSTool::connected( QgsGPSConnection *conn )
{
  mNmea = conn;
  connect( mNmea, SIGNAL( stateChanged( const QgsGPSInformation& ) ),
           this, SLOT( displayGPSInformation( const QgsGPSInformation& ) ) );
}

void QgsGPSTool::disconnectGps()
{
  delete mNmea;
  mNmea = NULL;
  if ( mpMapMarker )  // marker should not be shown on GPS disconnected - not current position
  {
    delete mpMapMarker;
    mpMapMarker = NULL;
  }
}

void QgsGPSTool::displayGPSInformation( const QgsGPSInformation& info )
{
  // set validity flag and status from GPS data
  // based on GGA, GSA and RMC sentences - the logic does not require all
  bool validFlag = false; // true if GPS indicates position fix
  FixStatus fixStatus = NoData;

  Q_UNUSED( fixStatus );

  // no fix if any of the three report bad; default values are invalid values and won't be changed if the corresponding NMEA msg is not received
  if ( info.status == 'V' || info.fixType == NMEA_FIX_BAD || info.quality == 0 ) // some sources say that 'V' indicates position fix, but is below acceptable quality
  {
    fixStatus = NoFix;
  }
  else if ( info.fixType == NMEA_FIX_2D ) // 2D indication (from GGA)
  {
    fixStatus = Fix2D;
    validFlag = true;
  }
  else if ( info.status == 'A' || info.fixType == NMEA_FIX_3D || info.quality > 0 ) // good
  {
    fixStatus = Fix3D;
    validFlag = true;
  }
  else  // unknown status (not likely)
  {
  }

  if ( validFlag )
  {
    validFlag = info.longitude >= -180.0 && info.longitude <= 180.0 && info.latitude >= -90.0 && info.latitude <= 90.0;
  }

  QgsPoint myNewCenter;
  if ( validFlag )
  {
    myNewCenter = QgsPoint( info.longitude, info.latitude );
  }
  else
  {
    myNewCenter = mLastGpsPosition;
  }

  // Avoid refreshing / panning if we havent moved
  if ( mLastGpsPosition != myNewCenter )
  {
    mLastGpsPosition = myNewCenter;

    if ( mRecenter )
    {
      QgsCoordinateReferenceSystem mypSRS = mpCanvas->mapRenderer()->destinationCrs();
      QgsCoordinateTransform myTransform( mWgs84CRS, mypSRS ); // use existing WGS84 CRS

      QgsPoint myPoint = myTransform.transform( myNewCenter );
      //keep the extent the same just center the map canvas in the display so our feature is in the middle
      QgsRectangle myRect( myPoint, myPoint );  // empty rect can be used to set new extent that is centered on the point used to construct the rect

      mpCanvas->setExtent( myRect );
      mpCanvas->repaint();

      mRecenter = false;
    }

    if ( mAutoAddVertices )
    {
      addVertex();
    }
  } // mLastGpsPosition != myNewCenter

  // new marker position after recentering
  if ( validFlag ) // update cursor position if valid position
  {                // initially, cursor isn't drawn until first valid fix; remains visible until GPS disconnect
    if ( ! mpMapMarker )
    {
      mpMapMarker = new QgsGpsMarker( mpCanvas );
    }

    // TODO use DPI
    mpMapMarker->setSize( 80 );
    mpMapMarker->setCenter( myNewCenter );
  }
}

void QgsGPSTool::addVertex()
{
  qDebug() << "Adding vertex: " << mCaptureList.size();

  if ( !mNmea )
  {
    return;
  }

  if ( !mpRubberBand )
  {
      createRubberBand( );
  }

  // we store the capture list in wgs84 and then transform to layer crs when
  // calling close feature
  mCaptureList.push_back( mLastGpsPosition );

  // we store the rubber band points in map canvas CRS so transform to map crs
  // potential problem with transform errors and wrong coordinates if map CRS is changed after points are stored - SLM
  // should catch map CRS change and transform the points
  QgsPoint myPoint;
  if ( mpCanvas && mpCanvas->mapRenderer() )
  {
    QgsCoordinateTransform t( mWgs84CRS, mpCanvas->mapRenderer()->destinationCrs() );
    myPoint = t.transform( mLastGpsPosition );
  }
  else
  {
    myPoint = mLastGpsPosition;
  }

  mpRubberBand->addPoint( myPoint );
}

void QgsGPSTool::createRubberBand( )
{
  QSettings settings;

  if ( mpRubberBand )
  {
    delete mpRubberBand;
  }
  mpRubberBand = new QgsRubberBand( mpCanvas, false );
  QColor color( settings.value( "/qgis/digitizing/line_color_red", 255 ).toInt(),
                settings.value( "/qgis/digitizing/line_color_green", 0 ).toInt(),
                settings.value( "/qgis/digitizing/line_color_blue", 0 ).toInt() );
  mpRubberBand->setColor( color );
  mpRubberBand->setWidth( settings.value( "/qgis/digitizing/line_width", 1 ).toInt() );
  mpRubberBand->show();
}

void QgsGPSTool::closeFeature()
{
  if ( !mNmea )
  {
    return;
  }

  QgsMapLayer *layer = mpCanvas->currentLayer();

  if ( !layer )
  {
    QMessageBox::information( mView, tr( "No current layer" ),
                              tr( "No current layer." ) );
    return;
  }

  QgsVectorLayer *vlayer = qobject_cast<QgsVectorLayer *>( mpCanvas->currentLayer() );

  if ( !vlayer )
  {
    QMessageBox::information( mView, tr( "No vector layer" ),
                              tr( "No vector layer." ) );
    return;
  }

    QGis::WkbType layerWKBType = vlayer->wkbType();

    // -------------- preconditions ------------------------
    // most of these preconditions are already handled due to the button being enabled/disabled based on layer geom type and editing capabilities, but not on valid GPS data

    //lines: bail out if there are not at least two vertices
    if ( layerWKBType == QGis::WKBLineString  && mCaptureList.size() < 2 )
    {
        QMessageBox::information( mView, tr( "Not enough vertices" ),
                                  tr( "Cannot close a line feature until it has at least two vertices." ) );
        return;
    }

    //polygons: bail out if there are not at least three vertices
    if ( layerWKBType == QGis::WKBPolygon && mCaptureList.size() < 3 )
    {
        QMessageBox::information( mView, tr( "Not enough vertices" ),
                                  tr( "Cannot close a polygon feature until it has at least three vertices." ) );
        return;
    }
    // -------------- end of preconditions ------------------------

    //
    // POINT CAPTURING
    //
    if ( layerWKBType == QGis::WKBPoint )
    {
        QgsFeature* f = new QgsFeature( 0, "WKBPoint" );

        int size = 0;
        char end = QgsApplication::endian();
        unsigned char *wkb = NULL;
        int wkbtype = 0;

        QgsCoordinateTransform t( mWgs84CRS, vlayer->crs() );
        QgsPoint myPoint = t.transform( mLastGpsPosition );
        double x = myPoint.x();
        double y = myPoint.y();

        size = 1 + sizeof( int ) + 2 * sizeof( double );
        wkb = new unsigned char[size];
        wkbtype = QGis::WKBPoint;
        memcpy( &wkb[0], &end, 1 );
        memcpy( &wkb[1], &wkbtype, sizeof( int ) );
        memcpy( &wkb[5], &x, sizeof( double ) );
        memcpy( &wkb[5] + sizeof( double ), &y, sizeof( double ) );

        f->setGeometryAndOwnership( &wkb[0], size );

        QgsFeatureAction action( tr( "Feature added" ), *f, vlayer, -1, -1, this );
        if ( action.addFeature() )
        {
            if ( mAutoCommit )
            {
                // should canvas->isDrawing() be checked?
                if ( !vlayer->commitChanges() ) //assumed to be vector layer and is editable and is in editing mode (preconditions have been tested)
                {
                    QMessageBox::information( mView,
                                              tr( "Error" ),
                                              tr( "Could not commit changes to layer %1\n\nErrors: %2\n" )
                                              .arg( vlayer->name() )
                                              .arg( vlayer->commitErrors().join( "\n  " ) ) );
                }

                vlayer->startEditing();
            }
        }

        delete f;
    } // layerWKBType == QGis::WKBPoint
    else // Line or polygon
    {
        mNmea->disconnect( this, SLOT( displayGPSInformation( const QgsGPSInformation& ) ) );

        //create QgsFeature with wkb representation
        QgsFeature* f = new QgsFeature( 0, "WKBLineString" );
        unsigned char* wkb;
        int size;
        char end = QgsApplication::endian();

        if ( layerWKBType == QGis::WKBLineString )
        {
            size = 1 + 2 * sizeof( int ) + 2 * mCaptureList.size() * sizeof( double );
            wkb = new unsigned char[size];
            int wkbtype = QGis::WKBLineString;
            int length = mCaptureList.size();
            memcpy( &wkb[0], &end, 1 );
            memcpy( &wkb[1], &wkbtype, sizeof( int ) );
            memcpy( &wkb[1+sizeof( int )], &length, sizeof( int ) );
            int position = 1 + 2 * sizeof( int );
            double x, y;
            for ( QList<QgsPoint>::iterator it = mCaptureList.begin(); it != mCaptureList.end(); ++it )
            {
                QgsPoint savePoint = *it;
                // transform the gps point into the layer crs
                QgsCoordinateTransform t( mWgs84CRS, vlayer->crs() );
                QgsPoint myPoint = t.transform( savePoint );
                x = myPoint.x();
                y = myPoint.y();

                memcpy( &wkb[position], &x, sizeof( double ) );
                position += sizeof( double );

                memcpy( &wkb[position], &y, sizeof( double ) );
                position += sizeof( double );
            }
            f->setGeometryAndOwnership( &wkb[0], size );
        }
        else if ( layerWKBType == QGis::WKBPolygon )
        {
            size = 1 + 3 * sizeof( int ) + 2 * ( mCaptureList.size() + 1 ) * sizeof( double );
            wkb = new unsigned char[size];
            int wkbtype = QGis::WKBPolygon;
            int length = mCaptureList.size() + 1;//+1 because the first point is needed twice
            int numrings = 1;
            memcpy( &wkb[0], &end, 1 );
            memcpy( &wkb[1], &wkbtype, sizeof( int ) );
            memcpy( &wkb[1+sizeof( int )], &numrings, sizeof( int ) );
            memcpy( &wkb[1+2*sizeof( int )], &length, sizeof( int ) );
            int position = 1 + 3 * sizeof( int );
            double x, y;
            QList<QgsPoint>::iterator it;
            for ( it = mCaptureList.begin(); it != mCaptureList.end(); ++it )
            {
                QgsPoint savePoint = *it;
                // transform the gps point into the layer crs
                QgsCoordinateTransform t( mWgs84CRS, vlayer->crs() );
                QgsPoint myPoint = t.transform( savePoint );
                x = myPoint.x();
                y = myPoint.y();

                memcpy( &wkb[position], &x, sizeof( double ) );
                position += sizeof( double );

                memcpy( &wkb[position], &y, sizeof( double ) );
                position += sizeof( double );
            }
            // close the polygon
            it = mCaptureList.begin();
            QgsPoint savePoint = *it;
            x = savePoint.x();
            y = savePoint.y();

            memcpy( &wkb[position], &x, sizeof( double ) );
            position += sizeof( double );

            memcpy( &wkb[position], &y, sizeof( double ) );
            f->setGeometryAndOwnership( &wkb[0], size );

            int avoidIntersectionsReturn = f->geometry()->avoidIntersections();
            if ( avoidIntersectionsReturn == 1 )
            {
                //not a polygon type. Impossible to get there
            }
            else if ( avoidIntersectionsReturn == 2 )
            {
                //bail out...
                QMessageBox::critical( mView, tr( "Error" ), tr( "The feature could not be added because removing the polygon intersections would change the geometry type" ) );
                delete f;
                connectGpsSlot();
                return;
            }
            else if ( avoidIntersectionsReturn == 3 )
            {
                QMessageBox::critical( mView, tr( "Error" ), tr( "An error was reported during intersection removal" ) );
                connectGpsSlot();
                return;
            }
        }
        // Should never get here, as preconditions should have removed any that aren't handled
        else // layerWKBType == QGis::WKBPolygon  -  unknown type
        {
            QMessageBox::critical( mView, tr( "Error" ), tr( "Cannot add feature. "
                                                         "Unknown WKB type. Choose a different layer and try again." ) );
            connectGpsSlot();
            return; //unknown wkbtype
        } // layerWKBType == QGis::WKBPolygon

        QgsFeatureAction action( tr( "Feature added" ), *f, vlayer, -1, -1, this );
        if ( action.addFeature() )
        {
            if ( mAutoCommit )
            {
                if ( !vlayer->commitChanges() ) //swiped... er... appropriated from QgisApp saveEdits()
                {
                    QMessageBox::information( mView,
                                              tr( "Error" ),
                                              tr( "Could not commit changes to layer %1\n\nErrors: %2\n" )
                                              .arg( vlayer->name() )
                                              .arg( vlayer->commitErrors().join( "\n  " ) ) );
                }

                vlayer->startEditing();
            }

            delete mpRubberBand;
            mpRubberBand = NULL;

            // delete the elements of mCaptureList
            mCaptureList.clear();
        } // action.addFeature()

        delete f;
        connectGpsSlot();
    } // layerWKBType == QGis::WKBPoint
    mpCanvas->refresh();  // NOTE: cancelling feature add refreshes canvas, OK does not; this may change, however, so do it anyway
}

void QgsGPSTool::connectGpsSlot( )
{
  connect( mNmea, SIGNAL( stateChanged( const QgsGPSInformation& ) ),
           this, SLOT( displayGPSInformation( const QgsGPSInformation& ) ) );
}
