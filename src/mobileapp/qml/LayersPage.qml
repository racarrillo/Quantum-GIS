import QtQuick 1.1
import Qt.labs.gestures 1.0
import org.qgis 1.0

Page {
    id: root

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
                color: "black"  // TODO stylish
                height: 50  // TODO stylish

                Text {
                    color: "red"; font.pointSize: 16; text: name
                }

                MouseArea {
                    anchors.fill: parent;
                    onClicked: {
                        layerOptions.visible = !layerOptions.visible
                        layerlistmodel.setCurrentLayer(index)
                    }
                }
            }

            Row {
                id: layerOptions
                width: parent.width
                anchors {
                    top: layerDescription.bottom
                    right: parent.right
                }
                visible: false
                spacing: 2

                Rectangle {
                    width: 50; height: 50;
                    color: (visibility) ? "red" : "blue"

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            layerlistmodel.setData(index, "visible", ! visibility);
                        }
                    }
                }

                Rectangle {
                    width: 50; height: 50;
                    color: (editable) ? "red" : "blue"

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            layerlistmodel.setData(index, "editable", ! editable);
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
            } // Row: layerOptions
        }
    } // Component: layerdelegate

    LayerListModel {
        id: layerlistmodel
        objectName: 'theLegend'
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
