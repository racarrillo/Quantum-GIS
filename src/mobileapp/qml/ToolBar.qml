import QtQuick 1.1

Rectangle {
    id: root
    property Item tools: null

    color: 'black'  // TODO styling
    height: 48*dp

    onToolsChanged: {
        tools.parent = root
        tools.visible = true
    }
}
