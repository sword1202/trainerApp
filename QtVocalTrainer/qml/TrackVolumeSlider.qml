import QtQuick 2.0
import QtGraphicalEffects 1.0

Item {
    width: 9
    height: 30

    property real volume: 1.0

    readonly property real maxY: height - circle.height

    Rectangle {
        height: parent.height
        width: 3
        radius: 1.5
        anchors.centerIn: parent
        color: "#DBDCE5"
    }

    DropShadow {
        source: circle
        anchors.fill: circle
        color: "#cc9DA3DC"
        radius: 7
    }

    Circle {
        id: circle
        y: 0
        circleRadius: parent.width / 2
        color: "white"

        MouseArea {
            anchors.fill: parent
            drag {
                target: parent
                maximumY: maxY
                minimumY: 0

                axis: Drag.YAxis
            }
        }

        onYChanged: {
            volume = (maxY - y) / maxY
        }
    }

    onVolumeChanged: {
        circle.y = (1.0 - volume) * (height - circle.height)
    }
}
