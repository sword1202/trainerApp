import QtQuick 2.0
import QtGraphicalEffects 1.0

Rectangle {
    width: 45
    height: 37.5
    color: "#00000000"

    property alias onImage : button.onImage
    property alias offImage : button.offImage
    property alias on: button.on

    ButtonShadow {
        anchors.fill: parent
        visible: !parent.on
        target: button.svg
    }

    ToggleButton {
        id: button
        anchors.fill: parent
    }
}
