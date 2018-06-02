import QtQuick 2.0

Rectangle {
    width: 121.75 + 8.07
    height: 13

    property var zoom: cpp.zoomController
    property int zoomIndex: zoom.zoom

    readonly property string circleColor: "#8790D0"
    readonly property string selectedCircleColor: "#8790D0"

    Rectangle {
        id: line
        width: 121.5
        height: 1
        color: "#8790D0"
        opacity: 0.4
        anchors.verticalCenter: zoomIndex0.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
    }

    Circle {
        id: zoomIndex0
        circleRadius: 1.875
        color: circleColor
        anchors.left: parent.left
        anchors.top: parent.top
    }

    Circle {
        id: zoomIndex1
        circleRadius: 1.875
        color: circleColor
        anchors.left: parent.left
        anchors.leftMargin: 11.25
        anchors.top: parent.top
    }

    Circle {
        id: zoomIndex2
        circleRadius: 1.875
        color: circleColor
        anchors.left: parent.left
        anchors.leftMargin: 27
        anchors.top: parent.top
    }

    Circle {
        id: zoomIndex3
        circleRadius: 1.875
        color: circleColor
        anchors.left: parent.left
        anchors.leftMargin: 48.75
        anchors.top: parent.top
    }

    Circle {
        id: zoomIndex4
        circleRadius: 1.875
        color: circleColor
        anchors.left: parent.left
        anchors.leftMargin: 78.75
        anchors.top: parent.top
    }

    Circle {
        id: zoomIndex5
        circleRadius: 1.875
        color: circleColor
        anchors.right: parent.right
        anchors.top: parent.top
    }

    MouseArea {
        anchors.fill: parent

        onClicked: {
            var x = mouse.x

            if (x < slider.minimumX) {
                x = slider.minimumX
            } else if(x > slider.maximumX) {
                x = slider.maximumX
            }

            slider.x = x
        }
    }

    SvgImage {
        id: slider
        property real minimumX: 0
        property real maximumX: parent.width

        x: 0
        width: 8.07
        height: 8.07
        anchors.bottom: parent.bottom

        source: "images/zoom_slider_triangle.svg"

        onXChanged: {
            var z = x - minimumX
            z *= (zoom.maxZoom - zoom.minZoom) / (maximumX - minimumX)
            cpp.zoomController.zoom = z + zoom.minZoom
        }

        MouseArea {
            anchors.fill: parent

            drag {
                target: parent
                minimumX: parent.minimumX
                maximumX: parent.maximumX
                axis: Drag.XAxis
            }
        }
    }
}
