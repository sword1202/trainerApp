import QtQuick 2.0
import QtGraphicalEffects 1.0

ToggleButton {
    id: root

    width: 45
    height: 37.5

    ButtonShadow {
        visible: root.state === "off"
        target: root.svg
    }
}
