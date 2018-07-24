import QtQuick 2.0

Rectangle {
    width: 51

    Piano {
        id: piano
        anchors.bottom: parent.bottom
        anchors.top: parent.top
        anchors.topMargin: 22
        anchors.left: parent.left
        width: parent.width
    }

    Rectangle {
        anchors.bottom: piano.top
        anchors.top: parent.top
        width: parent.width
    }

    HorizontalLine {
        anchors.bottom: piano.top
        anchors.bottomMargin: -0.5
        anchors.left: parent.left
        anchors.right: vertLine.left
    }

    VerticalLine {
        id: vertLine
        anchors.right: parent.right
        height: parent.height
        anchors.top: parent.top
        anchors.topMargin: 6
    }
}
