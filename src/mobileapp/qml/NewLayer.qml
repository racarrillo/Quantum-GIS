import QtQuick 1.1

Dialog {
    id: dialog
    title: "New layer"
    okbutton: false

    signal newSpatialiteLayer()

    ListView {
        anchors.fill: parent

        model: ListModel {
            ListElement { title: "Shapefile"; index: 0 }
            ListElement { title: "Spatialite"; index: 1 }
        }

        delegate: Rectangle {
            width: parent.width
            height: 48*dp
            color: "#f5f5f5"

            Text {
                anchors.verticalCenter: parent.verticalCenter
                color: "black"
                font.pixelSize: 24*dp
                text: title
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
                    dialog.visible = false;
                    switch (index) {
                        case 0:
                            console.log("new shapefile");
                            break;
                        case 1:
                            console.log("new spatialite");
                            break;
                    }
               }
            }
        }
    }

    onCancel: { dialog.visible = false }
}
