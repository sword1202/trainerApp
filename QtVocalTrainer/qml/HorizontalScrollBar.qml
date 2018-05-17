import QtQuick 2.0

Item {
    id: scrollBar

    // The properties that define the scrollbar's state.
    // position and pageSize are in the range 0.0 - 1.0.  They are relative to the
    // height of the page, i.e. a pageSize of 0.5 means that you can see 50%
    // of the height of the view.
    // orientation can be either Qt.Vertical or Qt.Horizontal
    property real position: 0
    property real positionIncrement: 0.01
    property real pageSize
    property real padding: 2.25
    property alias leftScroller: leftScroller
    property alias rightScroller: rightScroller

    height: 11.25

    // A light, semi-transparent background
    Rectangle {
        id: background
        anchors.fill: parent
        color: "#E8E7F0"
    }

    onPositionChanged: {
        stripe.x = position * mouseArea.drag.maximumX
    }

    // Size the bar to the required size, depending upon the orientation.
    Rectangle {
        id: stripe
        width: pageSize * parent.width
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.topMargin: padding
        anchors.bottomMargin: padding
        radius: height / 2

        MouseArea {
            id: mouseArea
            anchors.fill: parent
            drag {
                target: parent
                minimumX: 0
                maximumX: scrollBar.width - parent.width
                axis: Drag.XAxis
            }
        }

        onXChanged: {
            scrollBar.position = x / mouseArea.drag.maximumX
        }

        color: "#615F97"
    }

    Timer {
        interval: 30
        id: leftScroller
        triggeredOnStart: true
        repeat: true
        onTriggered: {
            if (position - positionIncrement >= 0) {
                position -= positionIncrement
            }
        }
    }

    Timer {
        interval: 30
        id: rightScroller
        triggeredOnStart: true
        repeat: true
        onTriggered: {
            if (position + positionIncrement <= 1.0) {
                position += positionIncrement
            }
        }
    }
}
