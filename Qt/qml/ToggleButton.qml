import QtQuick 2.0

Item {
    id: root

    property bool on: false

    property url onImage
    property url offImage
    property alias svg: svg

    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton

        onClicked: {
            root.on = !root.on
        }
    }

    SvgImage {
        anchors.fill: parent
        source: parent.on ? parent.onImage : parent.offImage
        id: svg
    }
}
