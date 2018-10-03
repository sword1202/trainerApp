import QtQuick 2.0

Item {
    VerticalScrollBar {
        pageSize: cpp.zoomController.pageSize
        anchors.fill: parent

        onPositionChanged: {
            cpp.zoomController.verticalScrollPosition = position
        }
    }
}
