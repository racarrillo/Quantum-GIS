import QtQuick 1.1

Rectangle {
    property alias okbutton: okbutton.visible
    property alias cancelbutton: cancelbutton.visible

    signal ok()
    signal cancel()

    height: 48*dp;
    width: parent.width

    anchors {
        bottom: parent.bottom
        left: parent.left
        right: parent.right
    }

    // Cancel button
    Rectangle {
        id: cancelbutton

        height: parent.height
        width: (okbutton.visible ? parent.width / 2 : parent.width) - 1*dp
        anchors {
            top: parent.top
            bottom: parent.bottom
            left: parent.left
        }
        color: "#f5f5f5"
        border { color: "#e9e9e9"; width: 2*dp }

        Text {
            anchors.centerIn: parent
            text: "Cancel"
            font.pixelSize: 16*dp
        }

        MouseArea {
            anchors.fill: parent
            onClicked: cancel()
        }
    }

    // Ok button
    Rectangle {
        id: okbutton

        height: parent.height
        width: (cancelbutton.visible ? parent.width / 2 : parent.width) - 1*dp
        // TODO anchor to cancelbutton?
        anchors {
            top: parent.top
            bottom: parent.bottom
            right: parent.right
        }
        color: "#f5f5f5"
        border { color: "#e9e9e9"; width: 2*dp }

        Text {
            anchors.centerIn: parent
            text: "Ok"
            font.pixelSize: 16*dp
        }

        MouseArea {
            anchors.fill: parent
            onClicked: ok()
        }
    }
}
