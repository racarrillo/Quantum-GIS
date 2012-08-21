/***************************************************************************
    NewSpatialiteLayerAttribues.qml  -  Attributes list for create a new spatialite layer
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
    id: root

    signal attributesChanged(variant attributes)

    GradientBackground {}

    DialogTitle {
        id: dialogtitle

        text: "Attributes"

        // Add attribute button
        ActionButton {
            id: addButton

            icon: visual.addIcon
            anchors.right: parent.right

            onClicked: newattribute.visible = true;
        }
    }

    // Model
    ListModel {
        id: attributesModel
    }

    // Delegate
    Component {
        id: attributeDelegate

        MenuElement {
            width: parent.width - 32*dp
            x: 16*dp

            Item {
                id: attributeName

                anchors.left: parent.left
                width: parent.width  / 2
                height: 48*dp

                Text {
                    color: visual.dialogForeground;
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 24*dp;
                    text: name
                }
            }

            Item {
                id: attributeType

                anchors.left: parent.right
                width: parent.width / 2
                height: 48*dp

                Text {
                    color: visual.dialogForeground;
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 24*dp;
                    text: type
                }

                ActionButton {
                    anchors.right: parent.right
                    icon: visual.removeIcon

                    onClicked: attributesModel.remove(index)
                }
            }
        }
    } // Component: attributedelegate

    ListView {
        width: parent.width
        anchors {
            top: dialogtitle.bottom
            bottom: okcancelbuttons.top
        }
        model: attributesModel
        delegate: attributeDelegate
    }

    OkCancelButtons {
        id: okcancelbuttons

        cancelbutton: false

        onOk:  {
            // Put the model's attributes into an array
            var attributes = [];

            for( var i = 0; i < attributesModel.count; i++ )
            {
              console.log( attributesModel.get(i).name + " " +  attributesModel.get(i).type )
              attributes.push(  attributesModel.get(i).name + " " +  attributesModel.get(i).type );
            }

            attributesChanged(attributes)
            root.visible = false
        }
    }

    NewSpatialiteLayerNewAttr {
        id: newattribute
        visible: false
    }

}
