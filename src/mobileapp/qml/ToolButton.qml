import QtQuick 1.1

Rectangle {
    id: button

    property alias text: label.text

    signal clicked

    width: 100; height: 80
    color: "#271d1d"
    border {color: "#B9C5D0"; width: 1}

    Text {
        id: label
        anchors.centerIn: parent
        font.pointSize: 9
        color: "white"
    }

    MouseArea {
        anchors.fill: parent
        onClicked: button.clicked()
    }
 }
