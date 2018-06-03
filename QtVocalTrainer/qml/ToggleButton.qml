import QtQuick 2.0

Item {
    id: root

    property bool on: false

    property string onImage
    property string offImage
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
