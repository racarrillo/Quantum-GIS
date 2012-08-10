import QtQuick 1.1
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
                height: 48*dp

                Text {
                    color: "red";
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 32*dp;
                    text: name
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
                spacing: 8*dp

                Rectangle {
                    width: 32*dp; height: 32*dp;
                    color: (visibility) ? "red" : "blue"

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            layerlistmodel.setData(index, "visible", ! visibility);
                        }
                    }
                }

                Rectangle {
                    width: 32*dp; height: 32*dp;
                    color: (editable) ? "red" : "blue"

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            layerlistmodel.setData(index, "editable", ! editable);
                        }
                    }
                }

                Rectangle {
                    width: 32*dp; height: 32*dp;
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
            anchors.centerIn: parent

            ToolButton {
                text: 'Add VLayer'
                height: 48*dp; width: 48*dp
                onClicked: addVectorLayer()
            }

            ToolButton {
                text: 'New VLayer'
                height: 48*dp; width: 48*dp
                onClicked: newVectorLayer()
            }

            ToolButton {
                text: 'Add RLayer'
                height: 48*dp; width: 48*dp
                onClicked: addRasterLayer()
            }
        }
    }
}
