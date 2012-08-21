/***************************************************************************
    GrandientBackground.qml  -  Grandient that disable mouse events
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
    anchors.fill: parent

    gradient: Gradient {
        GradientStop { position: 0.0; color: visual.screenBackgroundGradStart }
        GradientStop { position: 1.0; color: visual.screenBackgroundGradEnd }
    }

    MouseArea {
        anchors.fill: parent

        onClicked: { /* disable events on background */ }
    }
}
