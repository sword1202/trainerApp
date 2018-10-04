import QtQuick 2.0

Item {
    Zoom {
        id: zoom
    }

    Header {
        id: header
        width: parent.width
        objectName: "header"
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.right: parent.right
    }

    Rectangle {
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: header.bottom

        Text {
            anchors.top: parent.top
            anchors.topMargin: 14.25
            anchors.left: parent.left
            anchors.leftMargin: 22.5
            text: "The Beatles - Yesterday"
            color: "#24232D"
            font.family: "Lato"
            font.bold: true
            font.pointSize: 15
        }
    }

    ZoomSlider {
        zoom: zoom
        anchors.top: header.bottom
        anchors.topMargin: 15
        anchors.rightMargin: 23.25
        anchors.right: parent.right
    }
}
