import QtQuick 2.0
import "../"

Rectangle {
    id: root
    property string title: ""
    property url normalIcon: ""
    property url hoverIcon: ""
    property bool selected: false
    property var otherItems: []

    anchors.left: parent.left
    height: 65
    width: parent.width
    color: selected ? "#B7C0EF" : (mouseArea.containsMouse ? "#80B7C0EF" : "#00000000")

    SvgImage {
        id: icon
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 20
        source: selected ? hoverIcon : normalIcon
        width: 45
        height: 45
    }

    Text {
        text: title
        color: selected ? "#615F97" : "#24232D"
        font.pointSize: 14
        font.family: "Lato"
        anchors.left: icon.right
        anchors.leftMargin: 15
        anchors.verticalCenter: parent.verticalCenter
    }

    MouseArea {
        id: mouseArea
        hoverEnabled: true
        anchors.fill: parent

        onClicked: {
            root.selected = true
            for (var i = 0; i < otherItems.length; i++) {
                otherItems[i].selected = false
            }
        }
    }
}
