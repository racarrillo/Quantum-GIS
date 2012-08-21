/***************************************************************************
    MenuElement.qml  - For use in lists
     --------------------------------------
    Date                 : 31-Jul-2012
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

Rectangle {
    id: container

    signal clicked()
    signal pressAndHold()

    height: visual.menuElementHeight
    width: parent.width
    color: "transparent"

    Rectangle {
        anchors.bottom: parent.bottom
        color: visual.menuSeparator
        height: 2*dp
        width: parent.width
    }

    MouseArea {
        anchors.fill: parent

        onClicked: container.clicked()
        onPressAndHold: container.pressAndHold()

        // Visual feedback
        onPressed: {
            container.color = visual.highlighted
        }
        onReleased: container.color = "transparent"
        onExited: container.color = "transparent"
    }
}
