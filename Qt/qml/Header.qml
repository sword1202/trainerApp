import QtQuick 2.0

Rectangle {
    property alias playHead2: playerController.playHead2

    property alias tracksShowButton: tracksShowButton
    property alias lyricsShowButton: lyricsShowButton
    property alias tempo: tempo

    property real microphoneLevel: 0.0
    property real outputVolume: 0.0
    property real inputVolume: 0.0

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

        onOnChanged: {
            self.setShowLyrics(on)
        }
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

        Item {
            height: parent.height
            width: outputSlider.width + inputSlider.width +
                   inputSlider.anchors.leftMargin +
                   inputSliderIcon.width  + inputSliderIcon.anchors.leftMargin +
                   outputSliderIcon.width + outputSlider.anchors.leftMargin
            anchors.horizontalCenter: parent.horizontalCenter

            SvgImage {
                id: outputSliderIcon
                width: 17
                height: 26
                source: "images/output_level_slider.svg"
                anchors.bottom: inputSliderIcon.bottom
            }

            MicrophoneVolumeSlider {
                id: outputSlider
                volume: outputVolume

                anchors.left: outputSliderIcon.right
                anchors.leftMargin: 5.25
                anchors.verticalCenter: parent.verticalCenter

                onVolumeChanged: {
                    header.outputVolume = volume
                    self.onOutputVolumeChanged(volume)
                }
            }

            SvgImage {
                id: inputSliderIcon
                width: 12
                height: 22
                source: "images/input_level_slider.svg"
                anchors.left: outputSlider.right
                anchors.leftMargin: 27.75
                anchors.verticalCenter: parent.verticalCenter
            }

            MicrophoneVolumeSlider {
                id: inputSlider
                volume: inputVolume

                level: microphoneLevel
                anchors.left: inputSliderIcon.right
                anchors.leftMargin: 6.5
                anchors.verticalCenter: parent.verticalCenter

                onVolumeChanged: {
                    header.inputVolume = volume
                    self.onInputVolumeChanged(volume)
                }
            }
        }
    }
}
