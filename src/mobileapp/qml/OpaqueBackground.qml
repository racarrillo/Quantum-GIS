/***************************************************************************
    OpaqueBackground.qml  -  Transparent background
     --------------------------------------
    Date                 : 10-Jul-2012
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
    height: mainwindow.height
    width: mainwindow.width
    x: 0; y: 0
    color: "black"
    opacity: 0.5

    MouseArea {
        anchors.fill: parent

        // Hide the parent when the background is touched
        onClicked: {
            parent.parent.visible = false
        }
    }
}
