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

Dialog {
    id: container

    // layer's properties
    property int layerIndex: -1
    property alias layerName: container.title
    property alias layerVisible: visibleCheckbox.checked
    property alias layerEditable: editableCheckbox.checked

    // Reset the dialog
    onVisibleChanged: {
        if ( !visible ) {
            layerIndex = -1
            layerName = ""
            layerVisible = false
            layerEditable =  false
        }
    }

    title: layerName
    okbutton: false

    Column  {
        anchors.fill: parent

        MenuElement {
            Checkbox {
                id: visibleCheckbox

                anchors.fill: parent
                text: "Visible"
                checked: container.layerVisible
            }
        }

        MenuElement {
            Checkbox {
                id: editableCheckbox

                anchors.fill: parent
                text: "Editable"
                checked: container.layerEditable
            }
        }

    } // Column

    onCancel: container.visible = false
}
