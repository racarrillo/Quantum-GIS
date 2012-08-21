/***************************************************************************
    Button.qml  -  Button based on Android design guidelines
     --------------------------------------
    Date                 : 21-Jul-2012
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
    id: button

    property alias text: label.text

    signal clicked()
    signal pressAndHold()

    color: visual.buttonBackground
    height: visual.buttonHeight
    width: visual.buttonWidth

    Text {
        id: label
        anchors.centerIn: parent
        font.pixelSize: visual.buttonFontSize
        color: visual.buttonForeground
    }

    MouseArea {
        anchors.fill: parent
        onClicked: button.clicked()
        onPressAndHold: button.pressAndHold()
    }

    function activate() {
        container.color = visual.highlighted
    }

    function deactivate() {
        container.color = visual.buttonBackground
    }
}
