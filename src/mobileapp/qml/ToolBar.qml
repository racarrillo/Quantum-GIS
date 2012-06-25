import QtQuick 1.1

Rectangle {
    id: root
    property Item tools: null

    onToolsChanged: {
        tools.parent = root
        tools.visible = true
    }
}
