import QtQuick 1.1

Rectangle {
    id: mainmenu

    signal quit()

    height: 500; width: 200 // TODO styling

    ListModel {
        id: menumodel

        ListElement { title: "About"; action: "about" }
        ListElement { title: "..."; action: "" }
        ListElement { title: "Quit"; action: "quit" }
    }

    Component {
        id: menudelegate

        Rectangle {

            border { color: "black"; width: 1 }

            height: 48*dp; width: parent.width

            Text { text: title; font.pixelSize: 16*dp }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    switch (action) {
                        case "quit": quit(); break;
                    }
                }
            }
        }
    }

    ListView {
        height: parent.height; width: parent.width

        model: menumodel
        delegate: menudelegate
    }
}
