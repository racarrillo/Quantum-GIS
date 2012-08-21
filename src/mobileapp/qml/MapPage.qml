/***************************************************************************
    ActionBar.qml  -  Action bar based on Android design guidelines
     --------------------------------------
    Date                 : 24-Jul-2012
    Copyright            : (C) 2012 by Ramon Carrillo
    Email                : racarrillo91 at gmail.com
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

import QtQuick 1.1
import org.qgis 1.0

Page {
    id: canvas

    property MapCanvas mapCanvas: theMapCanvas
    property bool editing: false
    property bool gpsConnected: false

    signal addFeature();
    signal moveFeature();
    signal deleteFeature();
    signal touch();
    signal gpsConnect();
    signal gpsDisconnect();
    signal gpsAddVertex();
    signal gpsCloseFeature();
    signal gpsConnected();
    signal gpsDisconnected();

    onGpsConnect: canvas.gpsConnected = true
    onGpsDisconnect: canvas.gpsConnected = false

    Item {
        width: parent.width
        anchors.top: actionBar.bottom
        anchors.bottom: editTools.visible ? editTools.top : parent.bottom

        MapCanvas {
            id: theMapCanvas
            objectName: 'theMapCanvas' // the name is important
            size.width: parent.width; size.height: parent.height

            onRenderStarted: {
                busyindicator.visible = true;
            }

            onRenderCompleted: {
                busyindicator.visible = false;
            }
       }
    }

    AnimatedImage {
        id: busyindicator

        anchors.centerIn: parent
        source: "/images/loading.gif"
    }

    ActionBar {
        id: actionBar

        anchors.top: parent.top

        ActionButton {
            icon: "/images/icon"
            iconHeight: visual.actionBarHeight
            iconWidth: visual.actionBarHeight

            onClicked: mainmenu.visible = true
        }

        ActionButton {
            id: gpsbutton
            icon: visual.locationIcon
            anchors.right: layersbutton.left

            onClicked: gpsConnect()
            onPressAndHold: gpsDisconnect()
        }

        ActionButton {
            id: layersbutton
            icon: visual.layersIcon
            anchors.right: parent.right

            onClicked: {
                layersPage.visible = true
                canvas.visible = false
            }
        }
    }

    ActionBar {
        id: editTools
        visible: canvas.editing

        property ActionButton activeTool: null

        color: visual.menuBackground
        anchors.bottom: parent.bottom

        onActiveToolChanged: {
            if (activeTool == null) {
                touch();
            }
        }

        function enableTool(toolButton) {
            if (activeTool != null) {
                activeTool.deactivate()
            }
            activeTool = toolButton;
            toolButton.activate();
        }

        function disableTool(toolButton) {
            activeTool = null;
            toolButton.deactivate();
        }

        Row {
            anchors.centerIn: parent

            ActionButton {
                id: addFeatureButton
                icon: visual.addIcon
                width: editTools.width / 4
                onClicked: {
                    if (editTools.activeTool != null) {
                        if (editTools.activeTool == addFeatureButton) {
                            editTools.disableTool(addFeatureButton)
                            return;
                        }
                    }
                    editTools.enableTool(addFeatureButton);
                    addFeature()
                }
            }

            ActionButton {
                id: moveFeatureButton
                icon: visual.moveIcon
                width: editTools.width / 4
                onClicked: {
                    if (editTools.activeTool != null) {
                        if (editTools.activeTool == moveFeatureButton) {
                            editTools.disableTool(moveFeatureButton);
                            return;
                        }
                    }
                    editTools.enableTool(moveFeatureButton);
                    moveFeature()
                }
            }

            ActionButton {
                id: deleteFeatureButton
                icon: visual.removeIcon
                width: editTools.width / 4
                onClicked: {
                    if (editTools.activeTool != null) {
                        if (editTools.activeTool == deleteFeatureButton) {
                            editTools.disableTool(deleteFeatureButton);
                            return;
                        }
                    }
                    editTools.enableTool(deleteFeatureButton);
                    deleteFeature()
                }
            }

            ActionButton {
                id: addGpsVertextButton
                icon: visual.locationAddIcon
                width: editTools.width / 4

                onClicked: {
                    if ( canvas.gpsConnected ) {
                        gpsAddVertex()
                    } else {
                        mainwindow.displayToast("GPS connection not started")
                    }
                }

                onPressAndHold: {
                    if ( canvas.gpsConnected ) {
                       gpsCloseFeature()
                    } else {
                        mainwindow.displayToast("GPS connection not started")
                    }
                }
            }


        } // Row
    } // ActionBar
}
