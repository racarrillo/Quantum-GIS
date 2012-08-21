/***************************************************************************
    Spinner.qml  -  Spinner based on Android design guidelines
     --------------------------------------
    Date                 : 01-Aug-2012
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
    property alias model: menu.model
    property bool inlineLabel: true
    property color borderColor: visual.inputBorder

    signal inputChanged(int index)

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
    }

    // Input
    Rectangle {
        id: inputWrapper

        height: visual.inputHeight
        width: parent.width - 16*dp
        x: 8*dp
        y: inlineLabel ? 0 : labelWrapper.height
        color: "transparent"
        clip: true

        Text {
            id: input

            color: visual.inputForeground
            font.pixelSize: visual.smallFontSize
            anchors.verticalCenter: parent.verticalCenter
            text: "(unspecified)"
        }

    } // Input

    // Border
    Rectangle {
        anchors.bottom: parent.bottom
        height: 2*dp; width: parent.width
        color: container.borderColor
    }

    Rectangle {
        anchors.horizontalCenter: parent.right
        anchors.verticalCenter: parent.bottom

        height: 16*dp; width: 16*dp
        color: container.borderColor
        rotation: 45
    } // Border

    MouseArea {
        anchors.fill: parent

        onClicked: menu.visible = true
        onPressed: inputWrapper.color = visual.highlighted
        onReleased: inputWrapper.color = "transparent"
        onExited: inputWrapper.color = "transparent"
    }


    Menu {
        id: menu

        elementHeight: visual.inputHeight
        visible: false
        x: inputWrapper.x
        y: inputWrapper.y + inputWrapper.height  + 4*dp
        z: 1

        onValueChanged: {
            var value = model[index]

            input.text = value
            inputChanged(index)

            menu.visible = false
        }

        onFocusChanged: {
            console.log("focus: " + activeFocus)
            if ( !activeFocus )
                menu.visible = false
        }
    } // Menu
}
