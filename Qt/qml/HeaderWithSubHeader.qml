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
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 22.5
            text: "The Beatles - Yesterday"
            color: "#24232D"
            font.family: "LatoBold"
            font.bold: true
            font.pointSize: 13.5
        }
    }

    ZoomSlider {
        zoom: zoom
        anchors.top: header.bottom
        anchors.topMargin: 15
        anchors.rightMargin: 23.25
        anchors.right: parent.right
    }

//    Yardstick {
//        id: yardstick
//        anchors.bottom: parent.bottom
//        anchors.bottomMargin: 4.75
//        anchors.leftMargin: 67
//        anchors.left: parent.left
//        anchors.right: parent.right
//    }

//    VerticalLine {
//        anchors.right: yardstick.left
//        height: 15.75
//        anchors.bottom: parent.bottom
//    }

//    HorizontalLine {
//        width: parent.width
//        anchors.bottom: parent.bottom
//        anchors.left: parent.left
//    }
}
