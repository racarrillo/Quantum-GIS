/***************************************************************************
    OkCancelButtons.qml  -  Buttons for user input
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

Item {
    id: container

    property alias okbutton: okbutton.visible
    property alias cancelbutton: cancelbutton.visible
    property color color: "transparent"

    signal ok()
    signal cancel()

    Component.onCompleted: {
        okbutton.color = container.color
        cancelbutton.color = container.color
    }

    height: 48*dp;
    width: parent.width

    anchors {
        bottom: parent.bottom
        left: parent.left
        right: parent.right
    }

    Rectangle {
        id: topborder
        color: visual.dialogButtonBorder
        height: 2*dp
        width: parent.width
        anchors.top: parent.top
    }

    Rectangle {
        id: buttonseparator
        color: visual.dialogButtonBorder
        height: parent.height
        width: 2*dp
        anchors.left: cancelbutton.right
        visible: (okbutton.visible && cancelbutton.visible) ? true : false
    }

    // Cancel button
    Rectangle {
        id: cancelbutton

        height: parent.height
        width: (okbutton.visible ? parent.width / 2 : parent.width)
        anchors {
            top: topborder.bottom
            bottom: parent.bottom
            left: parent.left
        }

        Text {
            anchors.centerIn: parent
            text: "Cancel"
            color: visual.dialogButtonForeground
            font.pixelSize: 16*dp
        }

        MouseArea {
            anchors.fill: parent

            onClicked: cancel()
            onPressed: cancelbutton.color = visual.highlighted
            onReleased: cancelbutton.color = container.color
            onExited: cancelbutton.color = container.color
        }
    }

    // Ok button
    Rectangle {
        id: okbutton

        height: parent.height
        width: (cancelbutton.visible ? parent.width / 2 : parent.width)
        anchors {
            top: topborder.bottom
            bottom: parent.bottom
            right: parent.right
        }

        Text {
            anchors.centerIn: parent
            text: "Ok"
            color: visual.dialogButtonForeground
            font.pixelSize: 16*dp
        }

        MouseArea {
            anchors.fill: parent

            onClicked: ok()
            onPressed: okbutton.color = visual.highlighted
            onReleased: okbutton.color = container.color
            onExited: okbutton.color = container.color
        }
    }
}
