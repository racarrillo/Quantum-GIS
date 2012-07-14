import QtQuick 1.1
import org.qgis 1.0

Page {
    id: layers

    property MapCanvas canvas: null

    signal addVectorLayer()
    signal newVectorLayer()
    signal addRasterLayer()

    onCanvasChanged: {
        layerlistmodel.mapCanvas = canvas
    }

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
                    color: (visibility) ? "red" : "blue"

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            layerlistmodel.setData(index, "visibility", ! visibility);
                        }
                    }
                }

                Rectangle {
                    width: 50; height: 50;
                    color: "green"
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            layerlistmodel.removeLayer(index);
                        }
                    }
                }
            }
        }

    }

    LayerListModel {
        id: layerlistmodel
    }

    ListView {
        anchors.fill: parent
        model: layerlistmodel
        delegate: layerDelegate
    }

    ToolBar {

        anchors {
            bottom: parent.bottom
            left: parent.left
            right: parent.right
        }

        ToolBarLayout {

            ToolButton {
                text: 'Add VLayer'
                onClicked: addVectorLayer()
            }

            ToolButton {
                text: 'New VLayer'
                onClicked: newVectorLayer()
            }

            ToolButton {
                text: 'Add RLayer'
                onClicked: addRasterLayer()
            }
        }
    }
}
