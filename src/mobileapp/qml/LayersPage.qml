/***************************************************************************
    LayersPage.qml  -  Displays the loaded layers and its properties
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
    id: root

    property MapCanvas canvas: null

    onCanvasChanged: {
        layerlistmodel.mapCanvas = canvas
    }

    GradientBackground {}

    ActionBar {
        id: actionBar

        anchors.top: parent.top

        Row {
            anchors.left: parent.left

            ActionButton {
                id: backbutton
                icon: visual.previousIcon
                onClicked:  {
                  mapPage.visible = true
                  root.visible = false
                }
            }

            Item {
                height: parent.height
                width: title.width

                Text {
                    id: title

                    text: "Layers"
                    anchors.centerIn: parent
                    color: visual.actionBarForeground
                }
            }
        }

        Row {
            anchors.right: parent.right

            ActionButton {
                id: addbutton
                icon: visual.addIcon
                onClicked: addlayerdialog.visible = true
            }

            ActionButton {
                id: newbutton
                icon: visual.newIcon
                onClicked: newlayerdialog.visible = true
            }
        }
    }

    Component {
        id: layerDelegate

        MenuElement {
            width: parent.width - 32*dp
            x: 16*dp

            Text {
                color: visual.menuForeground
                anchors.verticalCenter: parent.verticalCenter
                text: name
            }

            ActionButton {
                anchors.right: parent.right
                icon: visual.removeIcon

                onClicked: {
                    layerlistmodel.removeLayer(index)
                }
            }

            onClicked: {
                layerProperties.layerIndex = index
                layerProperties.layerName = name
                layerProperties.layerVisible = visibility
                layerProperties.layerEditable = editable

                layerProperties.visible = true
            }

            onPressAndHold: {
                if ( editable ) {
                    layerlistmodel.setCurrentLayer(index)
                    mapPage.editing = true
                }
            }
        }
    } // layerdelegate

    LayerListModel {
        id: layerlistmodel
        objectName: 'theLegend'
    }

    ListView {
        width: parent.width

        anchors {
            top: actionBar.bottom
            bottom: parent.bottom
        }
        model: layerlistmodel
        delegate: layerDelegate
    }

    LayerProperties {
        id: layerProperties
        anchors.fill: parent

        visible: false

        onLayerVisibleChanged: {
            layerlistmodel.setData(layerIndex, "visible", layerVisible);
        }

        onLayerEditableChanged: {
            layerlistmodel.setData(layerIndex, "editable", layerEditable);
        }
    }
}
