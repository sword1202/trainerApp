import QtQuick 2.0
import "../"

Rectangle {
    property string title: ""
    property string icon: ""
    property string hoverIcon: ""
    property bool selected: false

    anchors.left: parent.left
    height: 65
    width: parent.width
    color: "#B7C0EF"
    opacity: selected ? 1.0 : 0.0

    SvgImage {
        id: icon
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 20
        source: selected ? hoverIcon : icon
        width: 45
        height: 45
    }

    Text {
        text: title
        color: "#615F97"
        font.pointSize: 14
        font.family: "Lato"
        anchors.left: icon.right
        anchors.leftMargin: 15
        anchors.verticalCenter: parent.verticalCenter
    }
}
