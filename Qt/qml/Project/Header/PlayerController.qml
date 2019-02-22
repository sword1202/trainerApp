import QtQuick 2.0
import QtGraphicalEffects 1.0
import "../../"

Item {
    property var playHead2

    width: 266.5
    height: 45

    SvgImage {
        id: bg
        anchors.fill: parent
        source: "images/player_background.svg"
    }

    ButtonShadow {
        target: bg
    }

    ToggleButton {
        id: bounds_selector

        anchors.right: parent.right
        height: parent.height
        width: 52.5

        onImage: "images/loop_on.svg"
        offImage: "images/loop_off.svg"

        onOnChanged: {
            self.setBoundsSelectionEnabled(on)
        }
    }

    ImageButton {
        id: go_back

        anchors.left: parent.left
        height: parent.height
        width: 52.5

        hoverImage: "images/go_back_hover.svg"
        idleImage: "images/go_back.svg"

        onClicked: {
            cpp.player.seekToPrevTact()
        }
    }

    ImageButton {
        id: go_forward

        anchors.left: go_back.right
        anchors.leftMargin: 1
        height: parent.height
        width: 52.5

        hoverImage: "images/go_forward_hover.svg"
        idleImage: "images/go_forward.svg"

        onClicked: {
            cpp.player.seekToNextTact()
        }
    }

    ImageButton {
        id: to_the_beginning

        anchors.left: go_forward.right
        anchors.leftMargin: 1
        height: parent.height
        width: 52.5

        hoverImage: "images/to_the_begining_hover.svg"
        idleImage: "images/to_the_begining.svg"

        onClicked: {
            cpp.player.stop()
            play.on = false
        }
    }

    ToggleButton {
        id: play

        anchors.left: to_the_beginning.right
        anchors.leftMargin: 1
        height: parent.height
        width: 52.5
        on: cpp.player.isPlaying

        onImage: "images/play_on.svg"
        offImage: "images/play_off.svg"

        Connections {
            target: cpp.player

            onComplete: {
                play.on = false
            }

            onIsPlayingChanged: {
                play.on = cpp.player.isPlaying
            }
        }

        onOnChanged: {
            if (on) {
                cpp.player.play();
            } else {
                cpp.player.pause();
            }
        }
    }
}
