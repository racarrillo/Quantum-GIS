/***************************************************************************
    TextField.qml  -  Text field based on Android design guidelines
     --------------------------------------
    Date                 : 29-Jul-2012
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

Item {
    id: container

    property alias label: label.text
    property alias text: input.text
    property bool inlineLabel: true
    property color borderColor: visual.inputBorder

    signal inputChanged()

    width: parent.width
    height: inlineLabel ? inputWrapper.height : labelWrapper.height + inputWrapper.height

    // Display the text if was setted from outside
    onTextChanged: {
        input.text = container.text
    }

    // Label
    Item {
        id: labelWrapper

        height: inlineLabel ? inputWrapper.height : visual.inputLabelHeight
        x: inlineLabel ? inputWrapper.x : 0
        y: 0

        Text {
            id: label

            color: visual.inputLabelForeground
            // The layout will depends on the inlineLabel property
            visible: (inlineLabel && input.text !== "") ? false : true
            font.pixelSize: inlineLabel ? input.font.pixelSize : 12*dp
            anchors.verticalCenter: parent.verticalCenter
        }
    }  // Label

    // Input
    Item {
        id: inputWrapper

        height: visual.inputHeight
        width: parent.width - 16*dp
        x: 8*dp
        y: inlineLabel ? 0 : labelWrapper.height

        TextInput {
            id: input

            color: visual.inputForeground
            font.pixelSize: visual.smallFontSize
            anchors.verticalCenter: parent.verticalCenter

            onFocusChanged: {
                if (activeFocus) {
                    container.borderColor = visual.highlighted
                } else {
                    container.borderColor = visual.menuSeparator
                }
                container.inputChanged()
            }
        }

    } // Input

    // Border
    Rectangle {
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        height: 8*dp; width: 1*dp
        color: container.borderColor
    }

    Rectangle {
        anchors.bottom: parent.bottom
        height: 1*dp; width: parent.width
        color: container.borderColor
    }

    Rectangle {
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        height: 8*dp; width: 1*dp
        color: container.borderColor
    } // Border

    MouseArea {
        anchors.fill: parent

        onClicked: {
            input.forceActiveFocus()
        }
    }
}
