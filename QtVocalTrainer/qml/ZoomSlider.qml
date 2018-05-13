import QtQuick 2.0

Rectangle {
    width: 123.75
    height: 15

    property var zoom
    property int zoomIndex: zoom.zoom

    readonly property string circleColor: "#8790D0"
    readonly property string selectedCircleColor: "#8790D0"

    readonly property var zoomIdesMap: [zoomIndex0, zoomIndex1, zoomIndex2, zoomIndex3, zoomIndex4, zoomIndex5]

    Rectangle {
        id: line
        width: 121.5
        height: 1
        color: "#8790D0"
        opacity: 0.4
        anchors.verticalCenter: zoomIndex0.verticalCenter
        anchors.left: parent.left
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

    SvgImage {
        property real minimumX: - width / 2 + zoomIndex0.circleRadius / 2 + 1

        width: 8.07
        height: 8.07
        anchors.bottom: parent.bottom
        source: "images/zoom_slider_triangle.svg"

        onMinimumXChanged: {
            x = minimumX
        }

        MouseArea {
            anchors.fill: parent

            drag {
                target: parent
                minimumX: parent.minimumX
                maximumX: line.width + parent.minimumX - 1.5
                axis: Drag.XAxis
            }
        }
    }
}
