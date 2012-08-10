import QtQuick 1.1
import org.qgis 1.0

Page {
    id: canvas

    property MapCanvas mapCanvas: theMapCanvas

    signal addFeature();
    signal moveFeature();
    signal deleteFeature();
    signal touch();

    MapCanvas {
        id: theMapCanvas
        objectName: 'theMapCanvas' // the name is important
        size.width: parent.width
        size.height: parent.height
    }

    /*
    ToolBar {
        width: parent.width
        height: 50

        anchors.bottom: canvas.bottom
    }
    */

    ToolBar {
        id: tools

        property ToolButton activeTool: null

        onActiveToolChanged: {
            if (activeTool == null) {
                touch();
            }
        }

        function enableTool(toolButton) {
            activeTool = toolButton;
            toolButton.color = "red";
        }

        function disableTool(toolButton) {
            activeTool = null;
            toolButton.color = "#271d1d";
        }

        anchors {
            bottom: parent.bottom
            left: parent.left
            right: parent.right
        }

        ToolBarLayout {

            ToolButton {
                id: addFeatureButton
                height: 48*dp; width: 48*dp;
                text: 'Add'
                onClicked: {
                    if (tools.activeTool != null) {
                        if (tools.activeTool == addFeatureButton) {
                            tools.disableTool(addFeatureButton)
                            return;
                        }
                    }
                    tools.enableTool(addFeatureButton);
                    addFeature()
                }
            }

            ToolButton {
                id: moveFeatureButton
                height: 48*dp; width: 48*dp;
                text: 'Move'
                onClicked: {
                    if (tools.activeTool != null) {
                        if (tools.activeTool == moveFeatureButton) {
                            tools.disableTool(moveFeatureButton);
                            return;
                        }
                    }
                    tools.enableTool(moveFeatureButton);
                    moveFeature()
                }
            }

            ToolButton {
                id: deleteFeatureButton
                height: 48*dp; width: 48*dp;
                text: 'Delete'
                onClicked: {
                    if (tools.activeTool != null) {
                        if (tools.activeTool == deleteFeatureButton) {
                            tools.disableTool(deleteFeatureButton);
                            return;
                        }
                    }
                    tools.enableTool(deleteFeatureButton);
                    deleteFeature()
                }
            }

        } // ToolBarLayout
    } // ToolBar
}
