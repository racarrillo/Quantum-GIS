/***************************************************************************
    Checkbox.qml  -  Checkbox based on Android design guidelines
     --------------------------------------
    Date                 : 13-Aug-2012
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

Item  {
    id: container

    property bool checked: false
    property bool disabled: false
    property alias text: label.text

    onCheckedChanged: {
        if ( !container.disabled )
            container.state = container.checked ? "NORMAL-CHECKED" : "NORMAL-UNCHECKED"
        else
            container.state = container.checked ? "DISABLED-CHECKED" : "DISABLED-UNCHECKED"
    }

    onDisabledChanged: {
        if ( !container.disabled )
            container.state = container.checked ? "NORMAL-CHECKED" : "NORMAL-UNCHECKED"
        else
            container.state = container.checked ? "DISABLED-CHECKED" : "DISABLED-UNCHECKED"
    }

    height: visual.inputHeight
    width: parent.width

    Item {
        anchors.left: parent.left
        height: visual.inputHeight
        width: visual.inputHeight

        Image {
            id: icon
            anchors.centerIn: parent
            source: visual.checkboxNormalUncheckedIcon
        }
    }

    Text {
        id: label

        color: visual.inputForeground
        font.pixelSize: visual.smallFontSize
        anchors.verticalCenter: parent.verticalCenter
        x: visual.inputHeight
    }

    MouseArea {
        anchors.fill: parent

        onClicked: {
            if ( container.disabled )
                return;

            container.checked = !container.checked
        }
    }

    states: [
        State {
            name: "NORMAL-CHECKED"
            //PropertyChanges { target: container; checked: true; disabled: false }
            PropertyChanges { target: icon; source: visual.checkboxNormalCheckedIcon }
        },
        State {
            name: "NORMAL-UNCHECKED"
            //PropertyChanges { target: container; checked: true; disabled: false }
            PropertyChanges { target: icon; source: visual.checkboxNormalUncheckedIcon }
        },
        State {
            name: "DISABLED-CHECKED"
            //PropertyChanges { target: container; checked: true; disabled: true }
            PropertyChanges { target: icon; source: visual.checkboxDisabledCheckedIcon }
        },
        State {
            name: "DISABLED-UNCHECKED"
            //PropertyChanges { target: container; checked: true; disabled: true }
            PropertyChanges { target: icon; source: visual.checkboxDisabledUncheckedIcon }
        }
    ]
}
