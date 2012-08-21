/***************************************************************************
    Dialog.qml  -  Dialog based on Android design guidelines
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
    id: dialog

    // The child elements added will be put in the 'content' element
    default property alias content: content.children
    property alias title: dialogtitle.text
    property alias okbutton: buttons.okbutton
    property alias cancelbutton: buttons.cancelbutton

    signal ok()
    signal cancel()

    OpaqueBackground { id: background  }

    Rectangle {
        anchors.centerIn: background
        height: mainwindow.height - 32*dp;
        width: mainwindow.width - 32*dp;
        color: visual.dialogBackground

        DialogTitle {
            id: dialogtitle
        }

        Item {
            id: content
            width: parent.width - 32*dp
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: dialogtitle.bottom
            anchors.bottom: buttons.top

            clip: true
        }

        OkCancelButtons {
            id: buttons
            anchors.bottom: parent.bottom

            onOk: dialog.ok()
            onCancel: dialog.cancel()
        }
    }
}
