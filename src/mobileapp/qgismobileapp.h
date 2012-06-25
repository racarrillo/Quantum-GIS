/***************************************************************************
                            qgismobileapp.h  -  description
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

#ifndef QGISMOBILEAPP_H
#define QGISMOBILEAPP_H

#include <qgsapplication.h>
#include <qgsmapcanvas.h>
#include <qgsmapcanvasproxy.h>
#include <QDeclarativeView>

class QgisMobileapp : public QObject
{
  Q_OBJECT
public:
  QgisMobileapp( QgsApplication *app );
  ~QgisMobileapp();

public slots:
  void addVectorLayer();
  void quit();

private:
  void updateCanvasLayerSet();

  QDeclarativeView mView;
  QgsMapCanvas *mMapCanvas;
  QgsMapCanvasProxy *mMapCanvasProxy;
  QList<QgsMapLayer *> mLayerSet;
};

#endif // QGISMOBILEAPP_H
