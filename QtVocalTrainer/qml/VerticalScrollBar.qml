import QtQuick 2.0

Item {
    id: scrollBar

    // The properties that define the scrollbar's state.
    // position and pageSize are in the range 0.0 - 1.0.  They are relative to the
    // height of the page, i.e. a pageSize of 0.5 means that you can see 50%
    // of the height of the view.
    // orientation can be either Qt.Vertical or Qt.Horizontal
    property real position: 0
    property real pageSize
    property real padding: 2.25

    width: 11.25

    // A light, semi-transparent background
    Rectangle {
        id: background
        anchors.fill: parent
        color: "#E8E7F0"
    }

    // Size the bar to the required size, depending upon the orientation.
    Rectangle {
        height: pageSize * parent.height
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: padding
        anchors.rightMargin: padding
        radius: width / 2

        MouseArea {
            id: mouseArea
            anchors.fill: parent
            drag {
                target: parent
                minimumY: 0
                maximumY: scrollBar.height - parent.height
                axis: Drag.YAxis
            }
        }

        onYChanged: {
            scrollBar.position = y / mouseArea.drag.maximumY
        }

        color: "#615F97"
    }
}
