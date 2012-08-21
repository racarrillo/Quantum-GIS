/***************************************************************************
    ActionButton.qml  -  Action button based on Android design guidelines
     --------------------------------------
    Date                 : 08-Aug-2012
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

    property alias icon: icon.source
    property alias iconHeight: icon.height
    property alias iconWidth: icon.width
    property bool disable: value


    signal clicked()
    signal pressAndHold()

    function activate() {
        highlight.visible = true
    }

    function deactivate() {
        highlight.visible = false
    }

    height: visual.actionBarHeight
    width: visual.actionBarHeight

    Rectangle {
        id: highlight

        anchors.fill: parent
        color: visual.highlighted
        visible: false
    }

    Image {
        id: icon

        source: "/images/icon"
        anchors.centerIn: parent
        smooth: true
    }

    MouseArea {
        anchors.fill: parent

        // Propagate events
        onClicked: container.clicked()
        onPressAndHold: container.pressAndHold()

        // Visual feedback
        onPressed: highlight.visible = true
        onReleased: highlight.visible = false
    }

}
