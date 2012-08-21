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
#include <qgsmaptoolzoom.h>
#include <qgsmaptoolpan.h>
#include <qgsmaptooltouch.h>

#include <QDeclarativeView>

// Touch gestures
#include <QGestureEvent>
#include <QTapAndHoldGesture>

class QgsLayerListModel;
class QgsNewSpatialiteLayerDialog;
class QgsGPSTool;

class QgisMobileapp : public QObject
{
    Q_OBJECT
public:
    QgisMobileapp( QgsApplication *app );
    ~QgisMobileapp();

    static QgisMobileapp *instance() { return smInstance; }

    QgsLayerListModel *legend();

public slots:
    void addVectorLayer();
    bool toggleEditing(QgsMapLayer *layer, bool allowCancel = true);
    void quit();

    //! activates the add feature tool
    void addFeature();
    //! activates the move feature tool
    void moveFeature();
    //! activates the delete feature tool
    void deleteFeature();
    //! Set map tool to Zoom out
    void zoomOut();
    //! Set map tool to Zoom in
    void zoomIn();
    //! Set map tool to pan
    void pan();
    //! Set map tool to touch
    void touch();
    //! Identify feature(s) on the currently selected layer

    void zoomFull();

private:
    void initDeclarative();
    void createActions();
    void createCanvasTools();
    void initLegend();

    QDeclarativeView mView;
    QgsMapCanvas *mMapCanvas;
    QgsLayerListModel *mLayerList;
    QgsMapCanvasProxy *mMapCanvasProxy;
    static QgisMobileapp *smInstance;


    class Tools
    {
    public:
        QgsMapTool* mZoomIn;
        QgsMapTool* mZoomOut;
        QgsMapTool* mPan;
        QgsMapTool* mTouch;
        QgsMapTool* mAddFeature;
        QgsMapTool* mMoveFeature;
        QgsMapTool* mDeleteFeature;
    } mMapTools;

    QgsNewSpatialiteLayerDialog *mNewSpatialiteLayer;
    QgsGPSTool *mGpsTool;

    bool gestureEvent( QGestureEvent *event );
    void tapAndHoldTriggered( QTapAndHoldGesture *gesture );
};

#endif // QGISMOBILEAPP_H
