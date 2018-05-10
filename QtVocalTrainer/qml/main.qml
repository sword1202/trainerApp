import QtQuick 2.11
import QtQuick.Window 2.11

Rectangle {
    id: window
    width: cpp.windowWidth
    height: cpp.windowHeight

    Rectangle {
        id: header
        height: 75
        width: parent.width
        color: "#c5cef8"

        SvgImage {
            id: lyricsHideButton
            source: "images/lyrics_hide_button.svg"
            anchors.verticalCenter: parent.verticalCenter

            x: 23
            width: 45
            height: 38
        }

        SvgImage {
            id: tracksHideButton
            source: "images/tracks_hide_button.svg"
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: lyricsHideButton.right
            anchors.leftMargin: 4

            width: 45
            height: 38
        }

        SvgImage {
            source: "images/metronome.svg"
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: tracksHideButton.right
            anchors.leftMargin: 4

            width: 45
            height: 38
        }
    }
}
