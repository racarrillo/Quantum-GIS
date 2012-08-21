/***************************************************************************
    Menu.qml  -  Menu on Android design guidelines
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

    property variant model: []
    property int elementHeight: visual.menuElementHeight

    signal valueChanged(int index)

    // Populate the list view
    onModelChanged: {
        for(var i = 0; i < container.model.length; i++) {
            menuModel.append( {"value":container.model[i] } )
        }
    }

    clip: true
    height: menuView.count * container.elementHeight
    width: 48*5*dp
    color: visual.menuBackground

    ListView {
        id: menuView

        anchors.fill: parent
        model: ListModel { id: menuModel }

        delegate: MenuElement {
            height: container.elementHeight

            Text {
                text: value

                anchors.verticalCenter: parent.verticalCenter
                color: visual.menuForeground
                x: 16*dp
            }

            onClicked: valueChanged(index)
        }
    }
}
