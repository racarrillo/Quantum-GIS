import QtQuick 1.1

Rectangle {
    id: mainmenu

    signal quit()

    height: 500; width: 200 // TODO styling

    ListModel {
        id: menumodel

        ListElement { text: 'About'; action: "about" }
        ListElement { text: '...'; action: "" }
        ListElement { text: 'Quit'; action: "quit" }
    }

    Component {
        id: menudelegate

        Rectangle {

            height: 50; width: parent.width // TODO styling

            Text { text: text }

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
