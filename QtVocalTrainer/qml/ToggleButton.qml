import QtQuick 2.0

Item {
    id: root
    state: "off"

    property string onImage
    property string offImage
    property alias svg: svg

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
}
