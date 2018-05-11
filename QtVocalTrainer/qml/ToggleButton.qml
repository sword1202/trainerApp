import QtQuick 2.0
import QtGraphicalEffects 1.0

Item {
    id: root
    state: "off"

    property string onImage
    property string offImage

    MouseArea {
        anchors.fill: parent
        onClicked: {
            root.state = root.state === "on" ? "off" : "on"
        }
    }

    states: [
        State {
            name: "on"
            PropertyChanges {
                target: svg
                source: onImage
            }
        },
        State {
            name: "off"
            PropertyChanges {
                target: svg
                source: offImage
            }
        }
    ]

    SvgImage {
        anchors.fill: parent
        id: svg
    }

    DropShadow {
        visible: root.state === "off"
        anchors.fill: svg

        horizontalOffset: 0
        verticalOffset: 2
        radius: 3.0
        color: "#aa9da3dc"
        source: svg
    }
}
