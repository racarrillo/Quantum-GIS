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
#include <qgssinglesymbolrenderer.h>
#include <qgsmaplayerregistry.h>
#include <qgsvectorlayer.h>
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
//#include "../app/qgsmaptooladdfeature.h"
//#include <qgsmaptooladdpart.h>
//#include <qgsmaptooladdring.h>
//#include <qgsmaptooladdvertex.h>
//#include <qgsmaptoolannotation.h>
//#include "qgsmaptooldeletering.h"
//#include "qgsmaptooldeletepart.h"
//#include "qgsmaptooldeletevertex.h"
//#include "qgsmaptoolfeatureaction.h"
//#include "qgsmaptoolformannotation.h"
//#include "qgsmaptoolidentify.h"
//#include "qgsmaptoolmeasureangle.h"
//#include "qgsmaptoolmovefeature.h"
//#include "qgsmaptoolmovevertex.h"
//#include "qgsmaptooloffsetcurve.h"
#include "qgsmaptoolpan.h"
//#include "qgsmaptoolselect.h"
//#include "qgsmaptoolselectrectangle.h"
//#include "qgsmaptoolselectfreehand.h"
//#include "qgsmaptoolselectpolygon.h"
//#include "qgsmaptoolselectradius.h"
//#include "qgsmaptoolreshape.h"
//#include "qgsmaptoolrotatepointsymbols.h"
//#include "qgsmaptoolsplitfeatures.h"
//#include "qgsmaptooltextannotation.h"
//#include "qgsmaptoolvertexedit.h"
#include "qgsmaptoolzoom.h"
//#include "qgsmaptoolsimplify.h"
//#include "qgsmeasuretool.h"
//#include "qgsmaptoolmovelabel.h"
//#include "qgsmaptoolrotatelabel.h"
//#include "qgsmaptoolchangelabelproperties.h"

#include "qgsmaptooltouch.h"


QgisMobileapp::QgisMobileapp( QgsApplication *app )
{
  Q_UNUSED(app);

  // Register QML custom types
  // TODO create a QDeclarativeExtensionPlugin and move this to it.
  qmlRegisterType<QgsMapCanvasProxy>("org.qgis", 1, 0, "MapCanvas");
  qmlRegisterType<QgsLayerListModel>("org.qgis", 1, 0, "LayerListModel");

  // Load QML main window
  mView.setSource(QUrl("qrc:/qml/qgsmobileapp.qml"));
  mView.setResizeMode(QDeclarativeView::SizeRootObjectToView);
  //mView.setGeometry(0, 0, QApplication::desktop()->width(), QApplication::desktop()->height());
  mView.setGeometry(0, 0, 480, 762);

  // Setup signals
  QObject *object = (QObject*)mView.rootObject();
  QObject::connect(object, SIGNAL(loadlayer()), this, SLOT(addVectorLayer()));
  QObject::connect(mView.engine(), SIGNAL(quit()), this, SLOT(quit()));

  // Get map canvas from QML
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

  createCanvasTools();

  mMapCanvas->setMapTool(mMapTools.mTouch);

  QObject::connect( mMapCanvas, SIGNAL( keyPressed(QKeyEvent*) ), this, SLOT( test() ) );

  // Show main window
  mView.show();

  return;
}

QgisMobileapp::~QgisMobileapp()
{
  // delete map layer registry and provider registry
  QgsApplication::exitQgis();
}

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
    qDebug("Layer is valid");
  }
  else
  {
    qDebug("Layer is NOT valid");
    return;
  }
  // Add the Vector Layer to the Layer Registry
  QList<QgsMapLayer *> layers;
  layers.append(mypLayer);
  QgsMapLayerRegistry::instance()->addMapLayers(layers, true);
}

void QgisMobileapp::createCanvasTools()
{
  // create tools
  mMapTools.mZoomIn = new QgsMapToolZoom( mMapCanvas, false /* zoomIn */ );
  mMapTools.mZoomOut = new QgsMapToolZoom( mMapCanvas, true /* zoomOut */ );
  mMapTools.mPan = new QgsMapToolPan( mMapCanvas );
  mMapTools.mTouch = new QgsMapToolTouch( mMapCanvas );
//  mMapTools.mIdentify = new QgsMapToolIdentify( mMapCanvas );
//  mMapTools.mFeatureAction = new QgsMapToolFeatureAction( mMapCanvas );
//  mMapTools.mMeasureDist = new QgsMeasureTool( mMapCanvas, false /* area */ );
//  mMapTools.mMeasureArea = new QgsMeasureTool( mMapCanvas, true /* area */ );
//  mMapTools.mMeasureAngle = new QgsMapToolMeasureAngle( mMapCanvas );
//  mMapTools.mTextAnnotation = new QgsMapToolTextAnnotation( mMapCanvas );
//  mMapTools.mFormAnnotation = new QgsMapToolFormAnnotation( mMapCanvas );
//  mMapTools.mAnnotation = new QgsMapToolAnnotation( mMapCanvas );
//  mMapTools.mAddFeature = new QgsMapToolAddFeature( mMapCanvas );
//  mMapTools.mMoveFeature = new QgsMapToolMoveFeature( mMapCanvas );
//  mMapTools.mReshapeFeatures = new QgsMapToolReshape( mMapCanvas );
//  mMapTools.mReshapeFeatures->setAction( mActionReshapeFeatures );
//  mMapTools.mSplitFeatures = new QgsMapToolSplitFeatures( mMapCanvas );
//  mMapTools.mSplitFeatures->setAction( mActionSplitFeatures );
//  mMapTools.mSelect = new QgsMapToolSelect( mMapCanvas );
//  mMapTools.mSelect->setAction( mActionSelect );
//  mMapTools.mSelectRectangle = new QgsMapToolSelectRectangle( mMapCanvas );
//  mMapTools.mSelectRectangle->setAction( mActionSelectRectangle );
//  mMapTools.mSelectPolygon = new QgsMapToolSelectPolygon( mMapCanvas );
//  mMapTools.mSelectPolygon->setAction( mActionSelectPolygon );
//  mMapTools.mSelectFreehand = new QgsMapToolSelectFreehand( mMapCanvas );
//  mMapTools.mSelectFreehand->setAction( mActionSelectFreehand );
//  mMapTools.mSelectRadius = new QgsMapToolSelectRadius( mMapCanvas );
//  mMapTools.mSelectRadius->setAction( mActionSelectRadius );
//  mMapTools.mAddRing = new QgsMapToolAddRing( mMapCanvas );
//  mMapTools.mAddRing->setAction( mActionAddRing );
//  mMapTools.mAddPart = new QgsMapToolAddPart( mMapCanvas );
//  mMapTools.mSimplifyFeature = new QgsMapToolSimplify( mMapCanvas );
//  mMapTools.mSimplifyFeature->setAction( mActionSimplifyFeature );
//  mMapTools.mDeleteRing = new QgsMapToolDeleteRing( mMapCanvas );
//  mMapTools.mDeleteRing->setAction( mActionDeleteRing );
//  mMapTools.mDeletePart = new QgsMapToolDeletePart( mMapCanvas );
//  mMapTools.mDeletePart->setAction( mActionDeletePart );
//  mMapTools.mNodeTool = new QgsMapToolNodeTool( mMapCanvas );
//  mMapTools.mNodeTool->setAction( mActionNodeTool );
//  mMapTools.mRotatePointSymbolsTool = new QgsMapToolRotatePointSymbols( mMapCanvas );
//  mMapTools.mRotatePointSymbolsTool->setAction( mActionRotatePointSymbols );
//  mMapTools.mMoveLabel = new QgsMapToolMoveLabel( mMapCanvas );
//  mMapTools.mMoveLabel->setAction( mActionMoveLabel );
//  mMapTools.mRotateLabel = new QgsMapToolRotateLabel( mMapCanvas );
//  mMapTools.mRotateLabel->setAction( mActionRotateLabel );
//  mMapTools.mChangeLabelProperties = new QgsMapToolChangeLabelProperties( mMapCanvas );
//  mMapTools.mChangeLabelProperties->setAction( mActionChangeLabelProperties );
  //ensure that non edit tool is initialised or we will get crashes in some situations
//  mNonEditMapTool = mMapTools.mPan;
//#ifdef HAVE_TOUCH
//  mNonEditMapTool = mMapTools.mTouch;
//#endif
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

void QgisMobileapp::zoomToSelected()
{
  mMapCanvas->zoomToSelected();
}

void QgisMobileapp::panToSelected()
{
  mMapCanvas->panToSelected();
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

void QgisMobileapp::addFeature()
{
  if ( mMapCanvas && mMapCanvas->isDrawing() )
  {
    return;
  }
  mMapCanvas->setMapTool( mMapTools.mAddFeature );
}

void QgisMobileapp::select()
{
  mMapCanvas->setMapTool( mMapTools.mSelect );
}

void QgisMobileapp::test()
{
  qDebug("test message");
}

void QgisMobileapp::quit()
{
  QgsApplication::quit();
}
