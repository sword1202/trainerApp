import QtQuick 2.0
import QtGraphicalEffects 1.0
import "../"

Rectangle {
    width: 67
    height: 111
    color: "white"

    HorizontalLine {
        width: parent.width
        anchors.left: parent.left
        anchors.top: parent.top
    }

    TrackVolumeSlider {
        id: vocalVolumeSlider
        anchors.left: parent.left
        anchors.topMargin: 26 - height / 2
        anchors.top: parent.top
        anchors.leftMargin: 45
    }

    TrackVolumeSlider {
        id: instrumentalVolumeSlider
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 38 - height / 2
        anchors.left: parent.left
        anchors.leftMargin: 45
    }

    Item {
        anchors.top: vocalVolumeSlider.top
        anchors.bottom: instrumentalVolumeSlider.bottom
        anchors.left: parent.left
        anchors.leftMargin: 16.5
        width: 16.5

        SvgImage {
            height: 13.5
            width: 16.5
            source: "speaker.svg"
            anchors.verticalCenter: parent.verticalCenter
        }
    }
}

