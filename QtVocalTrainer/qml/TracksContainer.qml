import QtQuick 2.0
import QtGraphicalEffects 1.0

Rectangle {
    property real volumeContainerWidth: value

    color: "#00000000"

    Rectangle {
        id: volumeContainer
        color: "white"
        height: parent.height
        width: volumeContainerWidth
        anchors.left: parent.left
    }

    TrackVolumeSlider {
        id: vocalVolumeSlider
        anchors.verticalCenter: vocal.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 45
    }

    TrackVolumeSlider {
        id: instrumentalVolumeSlider
        anchors.verticalCenter: instrumental.verticalCenter
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
            source: "images/speaker.svg"
            anchors.verticalCenter: parent.verticalCenter
        }
    }

    DropShadow {
        anchors.fill: vocal

        horizontalOffset: 0
        verticalOffset: 0
        radius: 30.0
        color: "#DDDBEE"
        source: vocal
    }

    Rectangle {
        id: vocal
        color: "#D2D0DD"
        anchors.bottom: instrumental.top
        anchors.bottomMargin: 12.75
        height: 30
        anchors.left: volumeContainer.right
        anchors.right: parent.right

        Rectangle {
            color: "white"
            height: parent.height
            anchors.left: parent.left
            width: 25.5
        }

        TrackButton {
            width: 67.5
            text: "Vocal track"
        }
    }

    Track {
        id: instrumental
        anchors.left: volumeContainer.right
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 14.25
        anchors.right: parent.right
    }
}

