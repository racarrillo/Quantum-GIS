/***************************************************************************
    qgismobile.app.qml  -  Main window
     --------------------------------------
    Date                 : 20-May-2012
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
import org.qgis 1.0

Window {
    id: mainwindow
    anchors.fill: parent; color: "black"

    MapPage {
        id: mapPage

        objectName: 'theMapPage'

        // Visible by default
        visible: true

        anchors.fill: parent
    }

    LayersPage {
        id: layersPage
        visible: false

        anchors.fill: parent

        canvas: mapPage.mapCanvas // Important!
    }

    AddLayer {
        id: addlayerdialog
        objectName: "theAddLayerDialog"
        anchors.fill: parent
        visible: false
    }

    NewLayer {
        id: newlayerdialog
        objectName: "theNewLayerDialog"
        anchors.fill: parent
        visible: false
    }

    NewSpatialiteLayer {
        id: newspatialitelayer
        objectName: "theNewSpatialiteLayerDialog"
        anchors.fill: parent
        visible: false
    }

    Item {
        id: mainmenu

        anchors.fill: parent
        visible: false

        OpaqueBackground {}

        Menu {
            anchors.centerIn: parent

            model: ["Settings", "About", "Quit"]

            onValueChanged: {
                switch( index ) {
                case 0:
                    console.log("settings")
                    break;
                case 1:
                    console.log("about");
                    break;
                case 2:
                    Qt.quit()
                    break;
                }
            }
        }
    } // mainmenu

    // The style
    HoloDark {
        id: visual
    }

    // TODO implement PageStack to manage pages?
    function activePage(page) {
        mapPage.visible = false;
        layersPage.visible = false;

        page.visible = true;
    }
}
