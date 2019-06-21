import QtQuick 2.0

Rectangle {
    id: root
    signal clicked
    property string text: "OK"
    property bool enabled: true

    color: "#615F97"
    height: 22.5
    width: 75
    radius: 3

    Text {
        anchors.centerIn: parent
        text: parent.text
        font.pointSize: 12
        font.family: "Lato"
        color: root.enabled ? "white" : "#d1d0db"
    }

    MouseArea {
        anchors.fill: parent

        onClicked: {
            if (parent.enabled) {
                parent.clicked()
            }
        }
    }
}
