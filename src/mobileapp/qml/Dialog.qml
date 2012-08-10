import QtQuick 1.1

Item {
    id: dialog

    default property alias content: content.children
    property string title
    property alias okbutton: buttons.okbutton
    property alias cancelbutton: buttons.cancelbutton

    signal ok()
    signal cancel()

    Rectangle {
        anchors.fill: parent
        color: "black"
        opacity: 0.5

        MouseArea {
            anchors.fill: parent

            onClicked: { /* disable events on the background */ }
        }
    }

    Rectangle {
        anchors.centerIn: parent
        height: 384*dp
        width: 256*dp
        color: "#f5f5f5"

        DialogTitle {
            id: dialogtitle
            visible: title === "" ? false : true
            z: 1
        }

        Item {
            id: content
            width: parent.width
            anchors.top: dialogtitle.bottom
            anchors.bottom: buttons.top
        }

        OkCancelButtons {
            id: buttons
            z: 1

            onOk: dialog.ok()
            onCancel: dialog.cancel()
        }
    }
}
