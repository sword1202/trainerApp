import QtQuick 2.0
import QtGraphicalEffects 1.0

ToggleButton {
    id: root

    width: 45
    height: 38

    DropShadow {
        visible: root.state === "off"
        anchors.fill: root.svg

        horizontalOffset: 0
        verticalOffset: 2
        radius: 3.0
        color: "#aa9da3dc"
        source: root.svg
    }
}
