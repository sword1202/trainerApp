import QtQuick 2.0

Item {
    property string color: "white"
    property real leftRadius: 0
    property real rightRadius: 0

    // left
    Rectangle {
        id: leftRect
        anchors.left: parent.left
        color: parent.color
        radius: leftRadius
        height: parent.height
        width: parent.width / 2 + rightRadius + leftRadius
    }

    // right
    Rectangle {
        color: parent.color
        anchors.left: leftRect.right
        anchors.leftMargin: -leftRadius - rightRadius
        anchors.right: parent.right
        radius: rightRadius
        height: parent.height
        width: parent.width / 2 + leftRadius + rightRadius
    }
}
