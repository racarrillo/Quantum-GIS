/***************************************************************************
    ActionBar.qml  -  Action bar based on Android design guidelines
     --------------------------------------
    Date                 : 22-Jul-2012
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

Page {
    id: container

    signal createDb()
    signal apply()
    signal reject()

    signal databaseChanged(string db)
    signal layerNameChanged(string layerName)
    signal primaryKeyChanged(bool primeryKey)
    signal geometryColumnChanged(string geometryColumn)
    signal typeChanged(string type)
    signal attributesChanged(variant attributes)

    GradientBackground {}

    DialogTitle {
        id: title
        text: "New spatialite layer"
        width: container.width
    }

    Flickable {
        anchors.top: title.bottom
        anchors.bottom: container.bottom
        boundsBehavior: Flickable.StopAtBounds
        width: parent.width - 32*dp
        contentHeight: controls.height
        contentWidth: parent.width - 32*dp
        clip: true
        x: 16*dp

        Column {
            id: controls

            width: container.width - 32*dp
            anchors.verticalCenter: container.verticalCenter
            spacing: 8*dp

            Spinner {
                id: layerdatabase
                label: "Database"
                inlineLabel: false
                width: parent.width

                MouseArea {
                    anchors.fill: parent

                    onClicked: {
                        databases.visible = true
                    }
                }
            }

            TextField {
                id: layername
                label: "Name"
                inlineLabel: false
                width: parent.width

                onInputChanged: {
                    layerNameChanged(text)
                }
            }

            TextField {
                id: layergeometrycolumn
                label: "Geometry column"
                text: "geometry"
                inlineLabel: false
                width: parent.width

                onInputChanged: {
                    geometryColumnChanged(text)
                }
            }

            Spinner {
                id: layertype
                label: "Type"
                inlineLabel: false
                width: parent.width
                model: ["Point","Linestring", "Polygon", "Multipoint", "Multilinestring", "Multipolygon" ]

                onInputChanged: {
                    typeChanged(text)
                }
            }

            // TODO delete this
            Item {
                height: 48*dp
                width: parent.width
            }

            Button {
                text: "Attributes"
                anchors.horizontalCenter: parent.horizontalCenter

                onClicked: {
                    attributeslist.visible = true
                }
            }

            // TODO delete this
            Item {
                height: 48*dp
                width: parent.width
            }

            // TODO anchor to bottom
            OkCancelButtons {
                id: okcancelbuttons

                onOk: {
                    var everythingisok = true
                    if (everythingisok) {
                        apply()
                        container.visible = false
                    }
                }

                onCancel: {
                    container.visible = false
                }
            }

         } // Column
    } // Flickable

    NewSpatialiteLayerDB {
        id: databases
        visible: false
        anchors.fill: parent

        onCreateDb: container.createDb()
        onSelectedDb: {
            layerdatabase.text = db
            container.databaseChanged(db)
        }
    }

    NewSpatialiteLayerAttributes {
        id: attributeslist
        visible: false
        anchors.fill: parent

        onAttributesChanged: {
            container.attributesChanged(attributes)
        }
    }
}
