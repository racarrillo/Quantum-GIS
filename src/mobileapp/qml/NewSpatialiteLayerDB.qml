/***************************************************************************
    NewSpatialiteLayerDB.qml  -  Select and create a spatialite database
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

Dialog {
    id: dialog
    title: "Databases"
    okbutton: false

    signal createDb()
    signal selectedDb(string db)

    ListView {
        id: list
        anchors.fill: parent

        model: spatialitedatabases

        delegate: MenuElement {
            Text {
                color: visual.dialogForeground
                anchors.verticalCenter: parent.verticalCenter
                text: modelData
            }

            onClicked: {
                dialog.visible = false
                selectedDb(modelData)
            }
        }

        footer: MenuElement {
            ActionButton {
                id: addButton

                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                icon: visual.addIcon
            }

            Text {
                anchors.left: addButton.right
                anchors.verticalCenter: parent.verticalCenter
                color: visual.dialogForeground
                text: "New database"
            }

            onClicked: {
                createDb()
            }
        }
    }

    onCancel: { visible = false }
}
