import QtQuick 2.0

Rectangle {
    property alias tracksShowButton: tracksShowButton
    property alias lyricsShowButton: lyricsShowButton

    id: header
    height: 75
    width: parent.width
    color: "#c5cef8"

    FeaturesToggleButton {
        id: lyricsShowButton
        onImage: "images/lyrics_show_button_on.svg"
        offImage: "images/lyrics_show_button_off.svg"

        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        anchors.leftMargin: 23.25
    }

    FeaturesToggleButton {
        id: tracksShowButton
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

    Player {
        anchors.centerIn: parent
    }

    Tempo {
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.rightMargin: 23.25
    }
}
