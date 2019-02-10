import QtQuick 2.0

import QtQuick 2.0
import QtGraphicalEffects 1.0
import "../../"

Item {
    width: 70
    height: 13

    property real volume: 1.0
    property real level: 0.0
    property bool blockXChanged: false
    property bool blockVolumeChanged: false

    readonly property real maxX: width - circle.width

    Rectangle {
        height: 7
        width: parent.width
        radius: height / 2
        anchors.centerIn: parent
        color: "white"

        // Canvas {
        //     anchors.left: parent.left
        //     anchors.right: parent.right
        //     anchors.leftMargin: 8
        //     anchors.rightMargin: 8

        //     anchors.top: parent.top
        //     anchors.bottom: parent.bottom
        //     anchors.topMargin: 1
        //     anchors.bottomMargin: 1

        //     onPaint: {
        //         var ctx = getContext("2d");
        //         ctx.clearRect(0, 0, parent.width, parent.height)
        //         ctx.fillStyle = "#31DD6C"
        //         var width = parent.width * level
        //         ctx.fillRect(0, 0, width, 2)
        //         ctx.fillRect(0, 3, width, 2)
        //     }

        //     Timer {
        //         interval: 1000 / 60
        //         running: true
        //         repeat: true

        //         onTriggered: {
        //             parent.requestPaint()
        //         }
        //     }
        // }

       Column {
           spacing: 1
           anchors.left: parent.left
           anchors.right: parent.right
           anchors.verticalCenter: parent.verticalCenter
           anchors.leftMargin: 8
           anchors.rightMargin: 8
           width: parent.width
           transform: Scale {
              xScale: level
           }

           Rectangle {
               height: 2
               width: parent.width
               anchors.left: parent.left
               color: "#31DD6C"
           }

           Rectangle {
               height: 2
               anchors.left: parent.left
               width: parent.width
               color: "#31DD6C"
           }
       }
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
            if (blockXChanged) {
                return
            }

            blockVolumeChanged = true
            volume = x / maxX
            blockVolumeChanged = false
        }
    }

    onVolumeChanged: {
        if (blockVolumeChanged) {
            return
        }

        blockXChanged = true
        circle.x = volume * (width - circle.width)
        blockXChanged = false
    }
}

