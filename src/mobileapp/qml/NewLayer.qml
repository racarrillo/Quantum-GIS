/***************************************************************************
    NewLayer.qml  -  Dialog for show a list of layers available for creation
     --------------------------------------
    Date                 : 30-Jul-2012
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

Dialog {
    id: dialog
    title: "New layer"
    okbutton: false

    signal newSpatialiteLayer()

    ListView {
        anchors.fill: parent
        clip: true

        model: ListModel {
            ListElement { title: "Shapefile"; index: 0 }
            ListElement { title: "Spatialite"; index: 1 }
        }

        delegate: MenuElement {

            Text {
                anchors.verticalCenter: parent.verticalCenter
                color: visual.dialogForeground
                font.pixelSize: 22*dp
                x: 8*dp
                text: title
            }

            onClicked: {
                dialog.visible = false;
                switch (index) {
                case 1:
                    newspatialitelayer.visible = true;
                    break;
                }
            }
        }
    }

    onCancel: { dialog.visible = false }
}
