import QtQuick 2.0

SvgImage {
    id: svg

    property string idleImage;
    property string hoverImage;

    state: "idle"

    states: [
        State {
            name: "idle"
            PropertyChanges {
                target: svg
                source: idleImage
            }
        },
        State {
            name: "hover"
            PropertyChanges {
                target: svg
                source: hoverImage
            }
        }
    ]

    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton

        onPressedChanged: {
            svg.state = pressed ? "hover" : "idle"
        }
    }
}
