import QtQuick 2.0
import QtGraphicalEffects 1.0

Item {
    width: 265.5
    height: 45

    Image {
        id: bg
        anchors.fill: parent
        source: "images/player_background.svg"
    }

    DropShadow {
        anchors.fill: bg

        horizontalOffset: 0
        verticalOffset: 2
        radius: 3.0
        color: "#aa9da3dc"
        source: bg
    }
}
