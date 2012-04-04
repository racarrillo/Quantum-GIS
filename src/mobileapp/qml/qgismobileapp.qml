import QtQuick 1.0

Rectangle {
    anchors.fill: parent; color: "black"

    Rectangle {
        anchors.centerIn: parent
        width: 100; height: 40; radius: 5; color: "lightgray"

        Text { anchors.centerIn: parent; text: "Quit"; color: "black" }

        MouseArea {
            anchors.fill: parent
            onClicked: Qt.quit()
        }
    }

    Image { source: "/images/icon" }
}
