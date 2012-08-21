/***************************************************************************
    qgsmaptooldeletefeature.cpp  -  map tool for deleting features touching them
     --------------------------------------
    Date                 : 24-Jul-2012
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

#include "qgsmaptooldeletefeature.h"
#include "qgsgeometry.h"
#include "qgslogger.h"
#include "qgsmapcanvas.h"
#include "qgsrubberband.h"
#include "qgsvectordataprovider.h"
#include "qgsvectorlayer.h"
#include "qgstolerance.h"
#include <QMessageBox>
#include <QMouseEvent>
#include <QSettings>
#include <limits>

QgsMapToolDeleteFeature::QgsMapToolDeleteFeature( QgsMapCanvas* canvas ): QgsMapToolEdit( canvas )
{
}

QgsMapToolDeleteFeature::~QgsMapToolDeleteFeature()
{
}

void QgsMapToolDeleteFeature::canvasReleaseEvent(QMouseEvent *e)
{
  QgsVectorLayer* vlayer = currentVectorLayer();
  if ( !vlayer )
  {
    return;
  }

  if ( !vlayer->isEditable() )
  {
    QMessageBox::information( 0, tr( "Layer not editable" ),
                              tr( "Cannot edit the vector layer. Use 'Toggle Editing' to make it editable." )
                            );
    return;
  }

  if ( !( vlayer->dataProvider()->capabilities() & QgsVectorDataProvider::DeleteFeatures ) )
  {
    QMessageBox::information( 0, tr( "Provider does not support deletion" ),
                              tr( "Data provider does not support deleting features" ) );
    return;
  }

  //find first geometry under mouse cursor and store iterator to it
  QgsPoint layerCoords = toLayerCoordinates( vlayer, e->pos() );
  double searchRadius = QgsTolerance::vertexSearchRadius( mCanvas->currentLayer(), mCanvas->mapRenderer() );
  QgsRectangle selectRect( layerCoords.x() - searchRadius, layerCoords.y() - searchRadius,
                           layerCoords.x() + searchRadius, layerCoords.y() + searchRadius );

  if ( vlayer->selectedFeatureCount() == 0 )
  {
    vlayer->select( QgsAttributeList(), selectRect, true );

    //find the closest feature
    QgsGeometry* pointGeometry = QgsGeometry::fromPoint( layerCoords );
    if ( !pointGeometry )
    {
      return;
    }

    double minDistance = std::numeric_limits<double>::max();

    QgsFeature cf;
    QgsFeature f;
    while ( vlayer->nextFeature( f ) )
    {
      if ( f.geometry() )
      {
        double currentDistance = pointGeometry->distance( *f.geometry() );
        if ( currentDistance < minDistance )
        {
          minDistance = currentDistance;
          cf = f;
        }
      }

    }

    delete pointGeometry;

    if ( minDistance == std::numeric_limits<double>::max() )
    {
      return;
    }

    vlayer->select( cf.id() );

    //display a warning
    if ( QMessageBox::warning( 0, tr( "Delete features" ), tr( "Delete the feature?" ), QMessageBox::Ok, QMessageBox::Cancel ) == QMessageBox::Cancel )
    {
      vlayer->deselect( cf.id() );
      return;
    }

    vlayer->beginEditCommand( tr( "Features deleted" ) );
    if ( !vlayer->deleteSelectedFeatures() )
    {
      QMessageBox::information( 0, tr( "Problem deleting features" ),
                                tr( "A problem occured during deletion of features" ) );
    }

    vlayer->endEditCommand();

    return;
  }

}
