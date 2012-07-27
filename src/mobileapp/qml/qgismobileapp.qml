import QtQuick 1.1
import org.qgis 1.0

Window {
    id: mainwindow
    anchors.fill: parent; color: "black"

    signal loadlayer()

    // Map tools
    signal addFeature()
    signal moveFeature()

    onRotationChanged: {
        console.log('rotation changed!')
    }

    Keys.onPressed: {
        console.log('key pressed')
    }

    Keys.onReleased: {
        console.log('key released')
    }

    ToolBar {
        id: mainbar

        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }

        ToolBarLayout {

            ToolButton {
                text: 'Q'
                onClicked: mainmenu.visible = !mainmenu.visible
            }

            ToolButton {
                text: 'Home'
                onClicked: activePage(welcomePage)
            }

            ToolButton {
                text: 'Legend'
                onClicked: activePage(layersPage)
            }

            ToolButton {
                text: 'Map'
                onClicked: activePage(mapPage)
            }

        }
    }

    WelcomePage {
        id: welcomePage
        visible: true
        anchors {
            top: mainbar.bottom
            left: mainwindow.left
            right: mainwindow.right
            bottom: mainwindow.bottom
        }
    }

    MapPage {
        id: mapPage

        objectName: 'theMapPage'

        visible: false
        anchors {
            top: mainbar.bottom
            left: mainwindow.left
            right: mainwindow.right
            bottom: mainwindow.bottom
        }

        //onAddFeature: addFeature()
        //onMoveFeature: moveFeature()
    }

    LayersPage {
        id: layersPage
        visible: false
        anchors {
            top: mainbar.bottom
            left: mainwindow.left
            right: mainwindow.right
            bottom: mainwindow.bottom
        }

        canvas: mapPage.mapCanvas // Important!

        onAddVectorLayer: loadlayer()
    }

    MainMenu {
        id: mainmenu
        visible: false

        onQuit: Qt.quit()

        anchors {
            top: mainbar.bottom
            left: mainwindow.left
        }
    }

    // TODO implement PageStack to manage pages?
    function activePage(page) {
        welcomePage.visible = false;
        mapPage.visible = false;
        layersPage.visible = false;

        page.visible = true;
    }
}
