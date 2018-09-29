import QtQuick 2.0

Rectangle {
    property string text: "OK"

    color: "#615F97"
    height: 22.5
    width: 75
    radius: 3

    Text {
        anchors.centerIn: parent
        text: parent.text
        font.pointSize: 12
        font.family: "Lato"
        color: "white"
    }
}
