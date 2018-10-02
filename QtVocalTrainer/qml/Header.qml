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
        on: true
        onImage: "images/lyrics_show_button_on.svg"
        offImage: "images/lyrics_show_button_off.svg"

        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        anchors.leftMargin: 23.25
    }

    FeaturesToggleButton {
        id: tracksShowButton
        on: true
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

        onOnChanged: {
            cpp.player.metronomeEnabled = on
        }
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
        id: tonalityController
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: tempo.left
        anchors.rightMargin: 3.75
    }

    Item {
        height: parent.height
        anchors.left: playerController.right
        anchors.right: tonalityController.left

        Row {
            height: parent.height
            spacing: 27.75
            anchors.horizontalCenter: parent.horizontalCenter

            MicrophoneVolumeSlider {
                anchors.verticalCenter: parent.verticalCenter
            }

            MicrophoneVolumeSlider {
                anchors.verticalCenter: parent.verticalCenter
            }
        }
    }
}
