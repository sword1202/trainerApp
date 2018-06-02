import QtQuick 2.0

Rectangle {
    property alias playHead2: playerController.playHead2

    property alias tracksShowButton: tracksShowButton
    property alias lyricsShowButton: lyricsShowButton
    property alias tempo: tempo

    id: header
    height: 75
    color: "#c5cef8"

    FeaturesToggleButton {
        id: lyricsShowButton
        state: "on"
        onImage: "images/lyrics_show_button_on.svg"
        offImage: "images/lyrics_show_button_off.svg"

        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        anchors.leftMargin: 23.25
    }

    FeaturesToggleButton {
        id: tracksShowButton
        state: "on"
        onImage: "images/tracks_show_button_on.svg"
        offImage: "images/tracks_show_button_off.svg"
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: lyricsShowButton.right
        anchors.leftMargin: 4
    }

    FeaturesToggleButton {
        onImage: "images/metronome_on.svg"
        offImage: "images/metronome_off.svg"
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: tracksShowButton.right
        anchors.leftMargin: 4
    }

    PlayerController {
        id: playerController
        anchors.centerIn: parent
    }

    Tempo {
        id: tempo
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.rightMargin: 23.25
    }

    TonalityController {
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: tempo.left
        anchors.rightMargin: 3.75
    }
}
