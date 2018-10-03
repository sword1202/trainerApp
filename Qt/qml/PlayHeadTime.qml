import QtQuick 2.0
import QtGraphicalEffects 1.0

Item {
    width: 39.5
    height: 18.25

    DropShadow {
        anchors.fill: rect
        source: rect
        color: "#8898D1"
        radius: 5
        opacity: 0.8
    }

    Rectangle {
        id: rect
        anchors.fill: parent
        color: "white"
        radius: 5
        anchors.bottomMargin: 3.75

        Text {
            text: "00:23"
            font.pointSize: 10.5
            anchors.centerIn: parent
            color: "#24232D"
            font.family: "LatoRegular"
        }
    }
}
