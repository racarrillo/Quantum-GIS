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

#ifndef QGSGPSTOOL_H
#define QGSGPSTOOL_H

#include <QObject>
#include <qgspoint.h>
#include <qgscoordinatereferencesystem.h>

class QgsGPSConnection;
class QgsGPSInformation;
class QgsGpsMarker;
class QgsMapCanvas;
class QgsRubberBand;

class QgsGPSTool: public QObject
{
    Q_OBJECT
  public:
    QgsGPSTool( QWidget *parent, QgsMapCanvas *canvas );
     ~QgsGPSTool();

  public slots:
    void connectGps();
    void disconnectGps();
    void addVertex();
    void closeFeature();

  private slots:
    void connected( QgsGPSConnection * );
    void timedout();
    void displayGPSInformation( const QgsGPSInformation& info );
    void connectGpsSlot();

  private:
    enum FixStatus  //GPS status
    {
      NoData, NoFix, Fix2D, Fix3D
    };
    void createRubberBand( );

    QWidget *mView;

    QgsGPSConnection *mNmea;
    QgsMapCanvas *mpCanvas;
    QgsGpsMarker *mpMapMarker;
    QgsRubberBand *mpRubberBand;
    QgsPoint mLastGpsPosition;
    QList<QgsPoint> mCaptureList;
    QgsCoordinateReferenceSystem mWgs84CRS;
    bool mRecenter;
    bool mAutoAddVertices;
    bool mAutoCommit;
};

#endif
