import QtQuick 2.0

Rectangle {
    property string text

    height: 15
    color: "white"
    border.color: "#514E64"
    border.width: 1
    radius: 2.25
    anchors.verticalCenter: parent.verticalCenter
    anchors.left: parent.left
    anchors.leftMargin: 12.5

    Text {
        anchors.centerIn: parent
        text: parent.text
        font.family: "LatoRegular"
        font.pixelSize: 9
        color: "#514E64"
    }
}
