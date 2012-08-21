/***************************************************************************
                            qgismobileapp.cpp  -  description
                              -------------------
              begin                : Wed Apr 04 10:48:28 CET 2012
              copyright            : (C) 2012 by Marco Bernasocchi
              email                : marco@bernawebdesign.ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <qgismobileapp.h>
#include <QApplication>

#include <QSettings>
#include <QMessageBox>
#include <QKeyEvent>
#include <QMetaObject>
#include <QString>
#include <QApplication>
#include <QWidget>
#include <QFileDialog>

#include <qgsapplication.h>
#include <qgsproviderregistry.h>
#include <qgsvectordataprovider.h>
#include <qgssinglesymbolrenderer.h>
#include <qgsmaplayerregistry.h>
#include <qgsvectorlayer.h>
#include <qgsrasterlayer.h>
#include <qgsmapcanvas.h>
#include <qgslogger.h>

// QML includes
#include <QtDeclarative> // for qmlRegisterType
#include <QDeclarativeView>
#include <QDeclarativeEngine>

// Custom QML types
#include <qgsmapcanvasproxy.h>
#include <qgslayerlistmodel.h>

//
// Map tools
//
#include "qgsmaptooladdfeature.h"
#include "qgsmaptoolmovefeature.h"
#include "qgsmaptooldeletefeature.h"
#include "qgsmaptoolpan.h"
#include "qgsmaptoolzoom.h"
#include "qgsmaptooltouch.h"

#include "qgsnewspatialitelayerdialog.h"
#include "qgsgpstool.h"

QgisMobileapp::QgisMobileapp( QgsApplication *app )
{
  Q_UNUSED(app);

  if ( smInstance )
    return;

  smInstance = this;

  initDeclarative();

  // Load QML main window
  mView.setSource(QUrl("qrc:/qml/qgsmobileapp.qml"));
  mView.setResizeMode(QDeclarativeView::SizeRootObjectToView);
  //mView.setGeometry(0, 0, QApplication::desktop()->width(), QApplication::desktop()->height());
  mView.setGeometry(0, 0, 480, 762);

  // Get map canvas from QML
  QObject *object = (QObject*)mView.rootObject();
  QgsMapCanvasProxy *mapCanvasProxy = object->findChild<QgsMapCanvasProxy *>("theMapCanvas");
  if (mapCanvasProxy == 0)
  {
    qDebug() << "Map canvas don't returned from QML";
    abort();
  }

  // Setup map canvas
  mMapCanvas = mapCanvasProxy->mapCanvas();
  mMapCanvas->freeze(false);
  mMapCanvas->setVisible(true);

  mNewSpatialiteLayer = new QgsNewSpatialiteLayerDialog(&mView);
  mGpsTool = new QgsGPSTool(&mView, mMapCanvas);

  initLegend();
  createActions();
  createCanvasTools();

  mMapCanvas->setMapTool(mMapTools.mTouch);


  // Show main window
  mView.show();

  return;
}

QgisMobileapp::~QgisMobileapp()
{
  delete mMapTools.mZoomIn;
  delete mMapTools.mZoomOut;
  delete mMapTools.mPan;
  delete mMapTools.mTouch;
  delete mMapTools.mAddFeature;
  delete mMapTools.mMoveFeature;

  delete mNewSpatialiteLayer;
  delete mGpsTool;

  // delete map layer registry and provider registry
  QgsApplication::exitQgis();
}

void QgisMobileapp::initDeclarative()
{
  // Register QML custom types
  // TODO create a QDeclarativeExtensionPlugin and move this to it.
  qmlRegisterType<QgsMapCanvasProxy>("org.qgis", 1, 0, "MapCanvas");
  qmlRegisterType<QgsLayerListModel>("org.qgis", 1, 0, "LayerListModel");

  int dpiX = QApplication::desktop()->physicalDpiX();
  int dpiY = QApplication::desktop()->physicalDpiY();
  int dpi = dpiX < dpiY ? dpiX : dpiY; // In case of asymetrical DPI. Improbable
  float dp = dpi * 0.00768443;

  mView.rootContext()->setContextProperty("dp", dp);
}

QgisMobileapp *QgisMobileapp::smInstance = 0;

void QgisMobileapp::addVectorLayer()
{
  QString myLayerPath         = QFileDialog::getOpenFileName( (QWidget *)&mView, tr( "Load SHP" ), ".", tr( "SHP files (*.shp *.SHP)" ) );
  QString myLayerBaseName     = QFileInfo(myLayerPath).baseName();
  QString myProviderName      = "ogr";

  QgsVectorLayer * mypLayer = new QgsVectorLayer(myLayerPath, myLayerBaseName, myProviderName);
  //QgsSingleSymbolRenderer *mypRenderer = new QgsSingleSymbolRenderer(mypLayer->geometryType());
  //mypLayer->setRenderer(mypRenderer);

  if (mypLayer->isValid())
  {
    qDebug("Vector layer is valid");
  }
  else
  {
    qDebug("Vector layer is NOT valid");
    delete mypLayer;
    return;
  }
  // Add the Vector Layer to the Layer Registry
  QList<QgsMapLayer *> layers;
  layers.append(mypLayer);
  QgsMapLayerRegistry::instance()->addMapLayers(layers, true);
}

void QgisMobileapp::addRasterLayer()
{


  QString myLayerPath = QFileDialog::getOpenFileName( (QWidget *)&mView );

  if ( mMapCanvas && mMapCanvas->isDrawing() )
  {
    return;
  }

  if ( myLayerPath == "" )
  {
    // no files selected
    mMapCanvas->freeze( false );
    return;
  }

  mMapCanvas->freeze( true );


  QString errMsg;

  if ( QgsRasterLayer::isValidRasterFileName( myLayerPath, errMsg ) )
  {
    QFileInfo myFileInfo(myLayerPath);
    //extract basename
    QString myBaseNameQString = myFileInfo.completeBaseName();

    // create the layer
    QgsRasterLayer *layer = new QgsRasterLayer( myLayerPath, myBaseNameQString );

    if ( shouldAskUserForGDALSublayers( layer ) )
    {
      // TODO support for sublayer
      delete layer;
      return;
    }
    else
    {
      // Add raster layer
      if ( layer->isValid() )
      {
        qDebug() << "Raster layer is valid";
      }
      else
      {
        qDebug() << "Raster layer is NOT valid";
        delete layer;
        return;

      }

      // register this layer with the central layers registry
      QList<QgsMapLayer *> myList;
      myList << layer;
      QgsMapLayerRegistry::instance()->addMapLayers( myList );
    }
  }
  else
  {
      QString msg = tr( "%1 is not a supported raster data source" ).arg( myLayerPath );

      if ( errMsg.size() > 0 )
        msg += "\n" + errMsg;

      QMessageBox::critical( 0, tr( "Unsupported Data Source" ), msg );
  }

  mMapCanvas->freeze( false );
  mMapCanvas->refresh();
}

// should the GDAL sublayers dialog should be presented to the user?
bool QgisMobileapp::shouldAskUserForGDALSublayers( QgsRasterLayer *layer )
{
    // return false if layer is empty or raster has no sublayers
    if ( !layer || layer->providerType() != "gdal" || layer->subLayers().size() < 1 )
        return false;

    QSettings settings;
    int promptLayers = settings.value( "/qgis/promptForRasterSublayers", 1 ).toInt();
    // 0 = always -> always ask (if there are existing sublayers)
    // 1 = if needed -> ask if layer has no bands, but has sublayers
    // 2 = never

  return promptLayers == 0 || ( promptLayers == 1 && layer->bandCount() == 0 );
}

void QgisMobileapp::createActions()
{

  QObject::connect(mView.engine(), SIGNAL(quit()), this, SLOT(quit()));

  QObject *object;

  // Add layers
  object = (QObject*)mView.rootObject();
  object = object->findChild<QObject *>("theAddLayerDialog");
  QObject::connect(object, SIGNAL(addVectorLayer()), this, SLOT(addVectorLayer()));
  QObject::connect(object, SIGNAL(addRasterLayer()), this, SLOT(addRasterLayer()));

  object = (QObject*)mView.rootObject();
  object = object->findChild<QObject *>("theMapPage");
  // Map tools
  QObject::connect( object, SIGNAL( addFeature() ), this, SLOT( addFeature()) );
  QObject::connect( object, SIGNAL( moveFeature() ), this, SLOT( moveFeature() ) );
  QObject::connect( object, SIGNAL( deleteFeature() ), this, SLOT( deleteFeature() ) );
  QObject::connect( object, SIGNAL( touch() ), this, SLOT( touch() ) );

  // Gps
  QObject::connect( object, SIGNAL( gpsConnect() ), mGpsTool, SLOT( connectGps() ) );
  QObject::connect( object, SIGNAL( gpsDisconnect() ), mGpsTool, SLOT( disconnectGps() ) );
  QObject::connect( object, SIGNAL( gpsAddVertex() ), mGpsTool, SLOT( addVertex() ) );
  QObject::connect( object, SIGNAL( gpsCloseFeature() ), mGpsTool, SLOT( closeFeature() ) );
}

void QgisMobileapp::createCanvasTools()
{
  // create tools
  mMapTools.mZoomIn = new QgsMapToolZoom( mMapCanvas, false /* zoomIn */ );
  mMapTools.mZoomOut = new QgsMapToolZoom( mMapCanvas, true /* zoomOut */ );
  mMapTools.mPan = new QgsMapToolPan( mMapCanvas );
  mMapTools.mTouch = new QgsMapToolTouch( mMapCanvas );
  mMapTools.mAddFeature = new QgsMapToolAddFeature( mMapCanvas );
  mMapTools.mMoveFeature = new QgsMapToolMoveFeature( mMapCanvas );
  mMapTools.mDeleteFeature = new QgsMapToolDeleteFeature( mMapCanvas );
}

void QgisMobileapp::initLegend()
{
  QObject *object = (QObject*)mView.rootObject();
  mLayerList = object->findChild<QgsLayerListModel *>("theLegend");
  if (mLayerList == 0)
  {
    qDebug() << "Layer list don't returned from QML";
    abort();
  }

  connect(mLayerList, SIGNAL(startEditingLayer(QgsMapLayer*)), this, SLOT(toggleEditing(QgsMapLayer*)));
}

void QgisMobileapp::addFeature()
{
  if ( mMapCanvas && mMapCanvas->isDrawing() )
  {
    return;
  }
  mMapCanvas->setMapTool( mMapTools.mAddFeature );

  QgsDebugMsg( "Setting map tool to add feature" );
}

void QgisMobileapp::moveFeature()
{
  mMapCanvas->setMapTool( mMapTools.mMoveFeature );

  QgsDebugMsg( "Setting map tool to move feature" );
}

void QgisMobileapp::deleteFeature()
{
  mMapCanvas->setMapTool( mMapTools.mDeleteFeature );

  QgsDebugMsg( "Setting map tool to delete feature" );
}

void QgisMobileapp::zoomIn()
{
  QgsDebugMsg( "Setting map tool to zoomIn" );

  mMapCanvas->setMapTool( mMapTools.mZoomIn );
}

void QgisMobileapp::zoomOut()
{
  mMapCanvas->setMapTool( mMapTools.mZoomOut );
}

void QgisMobileapp::pan()
{
  mMapCanvas->setMapTool( mMapTools.mPan );
}

void QgisMobileapp::touch()
{
  mMapCanvas->setMapTool( mMapTools.mTouch );
}

void QgisMobileapp::zoomFull()
{
  mMapCanvas->zoomToFullExtent();
}

bool QgisMobileapp::toggleEditing( QgsMapLayer *layer, bool allowCancel )
{
  QgsVectorLayer *vlayer = qobject_cast<QgsVectorLayer *>( layer );
  if ( !vlayer )
  {
    return false;
  }

  bool res = true;

  if ( !vlayer->isEditable() && !vlayer->isReadOnly() )
  {
    if ( !( vlayer->dataProvider()->capabilities() & QgsVectorDataProvider::EditingCapabilities ) )
    {
      QMessageBox::information( 0, tr( "Start editing failed" ), tr( "Provider cannot be opened for editing" ) );
      return false;
    }

    vlayer->startEditing();

    QSettings settings;
    QString markerType = settings.value( "/qgis/digitizing/marker_style", "Cross" ).toString();
    bool markSelectedOnly = settings.value( "/qgis/digitizing/marker_only_for_selected", false ).toBool();

    // redraw only if markers will be drawn
    if (( !markSelectedOnly || vlayer->selectedFeatureCount() > 0 ) &&
        ( markerType == "Cross" || markerType == "SemiTransparentCircle" ) )
    {
      vlayer->triggerRepaint();
    }
  }
  else if ( vlayer->isModified() )
  {
    QMessageBox::StandardButtons buttons = QMessageBox::Save | QMessageBox::Discard;
    if ( allowCancel )
      buttons |= QMessageBox::Cancel;

    switch ( QMessageBox::information( 0,
                                       tr( "Stop editing" ),
                                       tr( "Do you want to save the changes to layer %1?" ).arg( vlayer->name() ),
                                       buttons ) )
    {
      case QMessageBox::Cancel:
        res = false;
        break;

      case QMessageBox::Save:
        if ( !vlayer->commitChanges() )
        {
          QMessageBox::information( 0,
                                    tr( "Error" ),
                                    tr( "Could not commit changes to layer %1\n\nErrors: %2\n" )
                                    .arg( vlayer->name() )
                                    .arg( vlayer->commitErrors().join( "\n  " ) ) );
          // Leave the in-memory editing state alone,
          // to give the user a chance to enter different values
          // and try the commit again later
          res = false;
        }

        vlayer->triggerRepaint();
        break;

      case QMessageBox::Discard:
        if ( !vlayer->rollBack() )
        {
          QMessageBox::information( 0, tr( "Error" ), tr( "Problems during roll back" ) );
          res = false;
        }

        vlayer->triggerRepaint();
        break;

      default:
        break;
    }
  }
  else //layer not modified
  {
    vlayer->rollBack();
    res = true;
    vlayer->triggerRepaint();
  }

  #if 0
  if ( layer == activeLayer() )
  {
    activateDeactivateLayerRelatedActions( layer );
  }
  #endif

  return res;
}

void QgisMobileapp::quit()
{
  QgsApplication::quit();
}

QgsLayerListModel *QgisMobileapp::legend() {
  return mLayerList;
}
