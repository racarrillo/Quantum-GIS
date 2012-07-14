import QtQuick 1.1
import org.qgis 1.0

Page {
    id: canvas

    property MapCanvas mapCanvas: theMapCanvas

    MapCanvas {
        id: theMapCanvas
        objectName: 'theMapCanvas' // the name is important
        size.width: parent.width
        size.height: parent.height
    }

    ToolBar {
        width: parent.width
        height: 50

        anchors.bottom: canvas.bottom
    }

}
