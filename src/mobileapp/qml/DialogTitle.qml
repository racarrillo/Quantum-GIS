import QtQuick 1.1

Rectangle {
    property alias text: title.text

    height: 48*dp
    width: parent.width
    anchors.top: parent.top
    color: "#f5f5f5"

    Text {
        id: title
        anchors.centerIn: parent
        color: "black"
        font.pointSize: 16*dp
    }

    Rectangle {
        width: parent.width
        height: 2*dp
        anchors.bottom: parent.bottom
        color: "#30b4e5"
    }
}
