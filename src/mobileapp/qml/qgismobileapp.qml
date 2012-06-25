import QtQuick 1.1
import org.qgis 1.0

Window {
    id: mainwindow
    anchors.fill: parent; color: "black"

    signal loadlayer()

    onRotationChanged: {
        console.log('rotation changed!')
    }

    Keys.onPressed: {
        console.log('key pressed')
    }

    Keys.onReleased: {
        console.log('key released')
    }

    WelcomePage {
        id: welcome
        visible: true
        anchors.centerIn: parent
    }

    Page {
        id: canvas
        visible: false
        anchors.fill: parent

        MapCanvas {
            id: theMapCanvas
            objectName: 'theMapCanvas' // the name is important
            size.width: parent.width
            size.height: parent.height
        }
    }

    LayersPage {
        id: layers
        visible: false
        anchors.fill: parent

        Component {
            id: layerDelegate
            Rectangle {
                color: "black"
                border { color: "white"; width: 2 }
                width: parent.width
                height: layerDescription.height + ( (layerOptions.visible) ? layerOptions.height : 0 )

                Rectangle {
                    id: layerDescription
                    width: parent.width
                    color: "black"
                    height: 50

                    Text {
                        color: "red"; font.pointSize: 16; text: name
                    }

                    MouseArea {
                        anchors.fill: parent;
                        onClicked: {
                            layerOptions.visible = !layerOptions.visible
                        }
                    }
                }

                Row {
                    id: layerOptions
                    width: parent.width
                    anchors.top: layerDescription.bottom
                    visible: false

                    spacing: 2
                    Rectangle {
                        width: 50; height: 50;
                        color: "red"
                    }
                    Rectangle {
                        width: 50; height: 50;
                        color: "blue"
                    }
                    Rectangle {
                        width: 50; height: 50;
                        color: "green"
                    }
                }
            }

        }

        ListView {
            anchors.fill: parent
            model: LayerListModel { mapCanvas: theMapCanvas }
            delegate: layerDelegate
        }
    }

    ToolBar {

        // FIX positiong on bottom is not working
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }

        ToolBarLayout {

            ToolButton {
                text: 'Home'
                onClicked: activePage(welcome)
            }

            ToolButton {
                text: 'Layers'
                onClicked: activePage(layers)
            }

            ToolButton {
                text: 'Add'
                onClicked: mainwindow.loadlayer()
            }

            ToolButton {
                text: 'Map'
                onClicked: activePage(canvas)
            }

            ToolButton {
                text: 'Quit'
                onClicked: Qt.quit()
            }
        }
    }

    // TODO implement PageStack to manage pages?
    function activePage(page) {
        welcome.visible = false;
        canvas.visible = false;
        layers.visible = false;

        page.visible = true;
    }
}
