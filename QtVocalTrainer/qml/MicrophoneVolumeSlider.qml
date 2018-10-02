import QtQuick 2.0

import QtQuick 2.0
import QtGraphicalEffects 1.0

Item {
    width: 60
    height: 13.5

    property real volume: 1.0

    readonly property real maxX: width - circle.width

    Rectangle {
        height: 6.75
        width: parent.width
        radius: height / 2
        anchors.centerIn: parent
        color: "white"
    }

    MouseArea {
        anchors.fill: parent

        onClicked: {
            var x = mouse.x
            if (x > maxX) {
                x = maxX
            }

            circle.x = x - circle.width / 2
        }
    }

    DropShadow {
        source: circle
        anchors.fill: circle
        color: "#cc9DA3DC"
        radius: 7
    }

    Circle {
        id: circle
        x: 0
        circleRadius: parent.height / 2
        color: "white"

        MouseArea {
            anchors.fill: parent
            drag {
                target: parent
                maximumX: maxX
                minimumX: 0
                smoothed: true
                threshold: 1

                axis: Drag.XAxis
            }
        }

        onXChanged: {
            volume = (maxX - x) / maxX
        }
    }

    onVolumeChanged: {
        circle.x = (1.0 - volume) * (width - circle.width)
    }
}

