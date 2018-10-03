import QtQuick 2.0
import QtGraphicalEffects 1.0

DropShadow {
    property var target

    anchors.fill: target

    source: target
    color: "#9DA3DC"
    opacity: 0.8
    verticalOffset: 1.5
    radius: 3
}
