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
#include <QDeclarativeView>
#include <QDeclarativeEngine>

#include <QSettings>
#include <QMessageBox>
#include <QKeyEvent>
#include <QMetaObject>
#include <QString>
#include <QApplication>
#include <QWidget>

#include <qgsapplication.h>
#include <qgsproviderregistry.h>
#include <qgssinglesymbolrenderer.h>
#include <qgsmaplayerregistry.h>
#include <qgsvectorlayer.h>
#include <qgsmapcanvas.h>
#include <qgslogger.h>


QgisMobileapp::QgisMobileapp( QgsApplication *app, QWidget *parent, Qt::WFlags flags )
    : QMainWindow( parent, flags )
{
  // Create the Map Canvas
  mapCanvas = new QgsMapCanvas();
  mapCanvas->setObjectName(QString::fromUtf8("mapCanvas"));
//  mapCanvas->setExtent(mypLayer->extent());
  mapCanvas->enableAntiAliasing(true);
  mapCanvas->setCanvasColor( Qt::white );

  addLayer();

  mapCanvas->freeze(false);
  // Set the Map Canvas Layer Set
//  mapCanvas->setLayerSet(myLayerSet);
  mapCanvas->setVisible(true);
  mapCanvas->refresh();

  mapCanvas->show();


  mView.setSource(QUrl("qrc:/qml/qgsmobileapp.qml"));
  mView.setResizeMode(QDeclarativeView::SizeRootObjectToView);

  QObject::connect((QObject*)mView.engine(), SIGNAL(quit()), app, SLOT(quit()));

  mView.setGeometry(100,100, 800, 480);
  mView.show();

}

QgisMobileapp::~QgisMobileapp()
{

}

void QgisMobileapp::addLayer()
{
  QString myLayerPath         = "/tmp";
  QString myLayerBaseName     = "test";
  QString myProviderName      = "ogr";

  QgsVectorLayer * mypLayer = new QgsVectorLayer(myLayerPath, myLayerBaseName, myProviderName);
  QgsSingleSymbolRenderer *mypRenderer = new QgsSingleSymbolRenderer(mypLayer->geometryType());
  QList<QgsMapCanvasLayer> myLayerSet;
  mypLayer->setRenderer(mypRenderer);

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
  QgsMapLayerRegistry::instance()->addMapLayer(mypLayer, TRUE);

  // Add the Layer to the Layer Set
  myLayerSet.append(QgsMapCanvasLayer(mypLayer));
  // set the canvas to the extent of our layer
  mapCanvas->setExtent(mypLayer->extent());
  // Set the Map Canvas Layer Set
  mapCanvas->setLayerSet(myLayerSet);
}
