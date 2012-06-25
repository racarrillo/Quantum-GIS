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
  QString myLayerBaseName     = "test";
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

  mLayerSet << mypLayer;

  updateCanvasLayerSet();

  // set the canvas to the extent of our layer
  // TODO do this only if it's the first layer
  mMapCanvas->setExtent(mypLayer->extent());
}

void QgisMobileapp::updateCanvasLayerSet()
{
  QList<QgsMapCanvasLayer> canvasLayerSet;

  for( int i = 0; i < mLayerSet.size(); i++)
  {
    canvasLayerSet << QgsMapCanvasLayer( mLayerSet.at(i) );
  }

  mMapCanvas->setLayerSet(canvasLayerSet);

  mMapCanvas->update();
}

void QgisMobileapp::quit()
{
  QgsApplication::quit();
}
