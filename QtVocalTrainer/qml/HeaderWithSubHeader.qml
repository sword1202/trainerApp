import QtQuick 2.0

Item {
    Zoom {
        id: zoom
    }

    Header {
        id: header
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.right: parent.right
    }

    Rectangle {
        id: subheader
        height: 59.25
        color: "white"
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: header.bottom

        ZoomSlider {
            zoom: zoom
            anchors.top: parent.top
            anchors.topMargin: 15
            anchors.rightMargin: 23.25
            anchors.right: parent.right
        }

        HorizontalLine {
            width: parent.width
            anchors.bottom: parent.bottom
            anchors.left: parent.left
        }
    }

    Yardstick {
        id: yardstick
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
    }
}
