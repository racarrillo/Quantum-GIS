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

class QgisMobileapp : public QObject
{
    Q_OBJECT
public:
    QgisMobileapp( QgsApplication *app );
    ~QgisMobileapp();

public slots:
    void addVectorLayer();
    bool toggleEditing(QgsMapLayer *layer, bool allowCancel = true);
    void quit();
    void test();

private:
    void createActions();
    void createCanvasTools();
    void initLegend();

    QDeclarativeView mView;
    QgsMapCanvas *mMapCanvas;
    QgsMapCanvasProxy *mMapCanvasProxy;

    //! Set map tool to Zoom out
    void zoomOut();
    //! Set map tool to Zoom in
    void zoomIn();
    //! Set map tool to pan
    void pan();
    //! Set map tool to touch
    void touch();
    //! Identify feature(s) on the currently selected layer
    void identify();
    //! Measure distance
    void measure();
    //! Measure area
    void measureArea();
    //! Measure angle
    void measureAngle();

    void zoomToSelected();
    void panToSelected();
    void zoomFull();
    void zoomToPrevious();
    void zoomToNext();
    void addFeature();
    void select();

    class Tools
    {
    public:
        QgsMapTool* mZoomIn;
        QgsMapTool* mZoomOut;
        QgsMapTool* mPan;
        QgsMapTool* mTouch;
        QgsMapTool* mIdentify;
        QgsMapTool* mFeatureAction;
        QgsMapTool* mMeasureDist;
        QgsMapTool* mMeasureArea;
        QgsMapTool* mMeasureAngle;
        QgsMapTool* mAddFeature;
        QgsMapTool* mMoveFeature;
        QgsMapTool* mOffsetCurve;
        QgsMapTool* mReshapeFeatures;
        QgsMapTool* mSplitFeatures;
        QgsMapTool* mSelect;
        QgsMapTool* mSelectRectangle;
        QgsMapTool* mSelectPolygon;
        QgsMapTool* mSelectFreehand;
        QgsMapTool* mSelectRadius;
        QgsMapTool* mVertexAdd;
        QgsMapTool* mVertexMove;
        QgsMapTool* mVertexDelete;
        QgsMapTool* mAddRing;
        QgsMapTool* mAddPart;
        QgsMapTool* mSimplifyFeature;
        QgsMapTool* mDeleteRing;
        QgsMapTool* mDeletePart;
        QgsMapTool* mNodeTool;
        QgsMapTool* mRotatePointSymbolsTool;
        QgsMapTool* mAnnotation;
        QgsMapTool* mFormAnnotation;
        QgsMapTool* mTextAnnotation;
        QgsMapTool* mMoveLabel;
        QgsMapTool* mRotateLabel;
        QgsMapTool* mChangeLabelProperties;
    } mMapTools;

    bool gestureEvent( QGestureEvent *event );
    void tapAndHoldTriggered( QTapAndHoldGesture *gesture );
};

#endif // QGISMOBILEAPP_H
