/***************************************************************************
    AddLayer.qml  -  Dialog for show a list of layers available for adding
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
    title: "Add layer"
    okbutton: false

    signal addVectorLayer()
    signal addSpatialiteLayer()

    ListView {
        anchors.fill: parent
        clip: true

        model: ListModel {
            ListElement { title: "Vector layer"; index: 0 }
            ListElement { title: "Spatialite"; index: 1 }
        }

        delegate: MenuElement {

            Text {
                color: visual.dialogForeground
                text: title
                anchors.verticalCenter: parent.verticalCenter
                x: 8*dp
            }

            onClicked: {
                dialog.visible = false
                switch (index) {
                case 0:
                    addVectorLayer()
                    break;
                case 1:
                    addRasterLayer()
                    break;
                }
            }
        }
    }

    onCancel: { visible = false }
}
