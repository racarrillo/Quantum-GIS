import QtQuick 1.1

Rectangle {
    id: button

    property alias text: label.text

    signal clicked

    width: 100; height: 80  // TODO styling
    color: "#271d1d"  // TODO styling
    border {color: "#B9C5D0"; width: 1}  // TODO styling

    Text {
        id: label
        anchors.centerIn: parent
        font.pointSize: 9   // TOOD styling
        color: "white"  // TODO styling
    }

    MouseArea {
        anchors.fill: parent
        onClicked: button.clicked()
    }
 }
