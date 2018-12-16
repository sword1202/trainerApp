import QtQuick 2.0

Text {
    signal clicked
    property color normalColor: "#24232D"
    property color hoverColor: "#615F97"

    color: mouseArea.containsMouse ? hoverColor : normalColor
    font.pointSize: 18
    font.family: "Lato"

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true

        onClicked: {
            parent.clicked()
        }
    }
}
