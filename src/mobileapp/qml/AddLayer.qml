import QtQuick 1.1

Dialog {
    id: dialog
    title: "Add layer"
    okbutton: false

    signal addVectorLayer()
    signal addSpatialiteLayer()

    ListView {
        anchors.fill: parent

        model: ListModel {
            ListElement { title: "Vector layer"; index: 0 }
            ListElement { title: "Spatialite"; index: 1 }
        }

        delegate: Rectangle {
            width: parent.width
            height: 48*dp
            color: "#f5f5f5"

            Text {
                color: "black"
                text: title
                anchors.verticalCenter: parent.verticalCenter
                anchors.margins: 8*dp
            }

            Rectangle {
                width: parent.width
                height: 2*dp
                anchors.bottom: parent.bottom
                color: "#e9e9e9"
            }

            MouseArea {
                anchors.fill: parent

                onClicked: {
                    dialog.visible = false
                    switch (index) {
                        case 0:
                            addVectorLayer()
                            break;
                        case 1:
                            addSpatialiteLayer()
                            break;
                    }
               }
            }
        }
    }

    onCancel: { visible = false }
}
