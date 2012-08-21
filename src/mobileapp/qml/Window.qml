import QtQuick 1.0

Rectangle {

    function displayToast(message) {
        toastMessage.text = message
        toast.opacity = 1
    }

    // Toast
    Rectangle {
        id: toast

        anchors.horizontalCenter: parent.horizontalCenter
        color: visual.toastBackground
        height: 40*dp;
        width:  ( (toastMessage.width + 16*dp) <= 192*dp ) ? 192*dp : toastMessage.width + 16*dp
        opacity: 0
        radius: 2*dp
        y: parent.height - 112*dp
        z: 1

        Behavior on opacity {
            NumberAnimation { duration: 500 }
        }

        // Visible only for 3 seconds
        onOpacityChanged: {
          toast.visible = (toast.opacity > 0)

          if ( toast.opacity == 1 ) {
              toastTimer.start()
          }
        }

        Text {
            id: toastMessage

            anchors.centerIn: parent
            font.pixelSize: visual.toastFontSize
            color: visual.toastForeground
        }

        // Visible only for 3 seconds
        Timer {
              id: toastTimer

              interval: 3000
              onTriggered: { toast.opacity = 0 }
        }

    }
}
