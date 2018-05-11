import QtQuick 2.11
import QtQuick.Window 2.11
import QtGraphicalEffects 1.0

Rectangle {
    id: window
    width: cpp.windowWidth
    height: cpp.windowHeight

    Rectangle {
        id: header
        height: 75
        width: parent.width
        color: "#c5cef8"

        FeaturesToggleButton {
            id: lyricsHideButton
            onImage: "images/lyrics_show_button_on.svg"
            offImage: "images/lyrics_show_button_off.svg"

            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            anchors.leftMargin: 23
        }

        FeaturesToggleButton {
            id: tracksHideButton
            onImage: "images/tracks_show_button_on.svg"
            offImage: "images/tracks_show_button_off.svg"
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: lyricsHideButton.right
            anchors.leftMargin: 4
        }

        FeaturesToggleButton {
            onImage: "images/metronome_on.svg"
            offImage: "images/metronome_off.svg"
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: tracksHideButton.right
            anchors.leftMargin: 4
        }

        Player {
            anchors.centerIn: parent
        }
    }
}
