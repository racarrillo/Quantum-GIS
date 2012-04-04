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

#include "qgismobileapp.h"
#include <QApplication>
#include <QDeclarativeView>
#include <QDeclarativeEngine>

#include <QSettings>
#include <QMessageBox>
#include <QKeyEvent>
#include <QMetaObject>

#include "qgsapplication.h"
#include "qgsdataitem.h"
#include "qgsencodingfiledialog.h"
#include "qgsgenericprojectionselector.h"
#include "qgslogger.h"
#include "qgsmaplayerregistry.h"
#include "qgsproviderregistry.h"
#include "qgsvectorlayer.h"
#include "qgsrasterlayer.h"
#include "qgsnewvectorlayerdialog.h"
#include "qgsattributetablemodel.h"
#include "qgsattributetablefiltermodel.h"


QgisMobileapp::QgisMobileapp( QgsApplication *app, QWidget *parent, Qt::WFlags flags )
    : QMainWindow( parent, flags )
{
  mView.setSource(QUrl("qrc:/qml/qgsmobileapp.qml"));
  mView.setResizeMode(QDeclarativeView::SizeRootObjectToView);

  QObject::connect((QObject*)mView.engine(), SIGNAL(quit()), app, SLOT(quit()));

  mView.setGeometry(100,100, 800, 480);
  mView.show();
}

QgisMobileapp::~QgisMobileapp()
{

}
