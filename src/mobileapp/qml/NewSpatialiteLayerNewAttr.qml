/***************************************************************************
    NewSpatialiteLayerNewAttr.qml  -  Add new attributes for the spatialite layer
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
    id: container

    title: "New attribute"

    onVisibleChanged: {
        attrName.text = "";
        attrType.text = "";
    }

    Column {
        anchors.fill: parent

        TextField {
            id: attrName

            label: "Name"
            width: parent.width
        }

        Spinner {
            id: attrType

            label: "Type"
            width: parent.width
            model: ["Text", "Number", "Real"]
        }

    } // Column

    onCancel: {
        container.visible = false
    }

    onOk: {
        attributesModel.append({
            "name": attrName.text,
            "type": attrType.text.toLowerCase()
        });
        container.visible = false
    }
}
