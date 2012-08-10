import QtQuick 1.1

Rectangle {
    id: button

    property alias text: label.text

    signal clicked

    color: "#271d1d"  // TODO styling
    border {color: "#B9C5D0"; width: 1}  // TODO styling

    Text {
        id: label
        anchors.centerIn: parent
        font.pixelSize: 16*dp
        color: "white"  // TODO styling
    }

    MouseArea {
        anchors.fill: parent
        onClicked: button.clicked()
    }
 }
