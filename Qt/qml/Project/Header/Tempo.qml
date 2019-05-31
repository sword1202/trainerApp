import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import "../../js/strings.js" as Strings
import QtGraphicalEffects 1.0
import "../../"

Item {
    id: root
    property int tempo: cpp.player.source ? cpp.player.beatsPerMinute : -1
    property int tempoFactorIncrement: 0
    property int minimumTempoFactorIncrement: -19
    property int maximumTempoFactorIncrement: 100
    property string tempoFactorText: "1.00x"

    width: cpp.player.isRecording ? 126.5 - 18.75 - 1 : 126.5
    height: 38.25

    Component.onCompleted: {

    }

    onTempoFactorIncrementChanged: {
        var increment = 0.05
        tempoFactorText = (1.0 + increment * tempoFactorIncrement).toFixed(2)
        var newTempoFactor = parseFloat(tempoFactorText)
        tempoFactorText += "x"
        //cpp.player.tempoFactor = newTempoFactor
    }

    onTempoChanged: {
        tempoText.text = tempo > 0 ? tempo.toString() : "---"
    }

    DropShadow {
        anchors.fill: bg
        source: bg
        color: "#9DA3DC"
        opacity: 0.8
        verticalOffset: 1.5
        radius: 3
    }

    Rectangle {
        id: bg
        radius: 7.5
        color: "#C2CFFE"
        anchors.fill: parent
    }

    SvgImage {
        anchors.left: parent.left
        id: left
        width: 54
        height: parent.height
        source: "images/tonality_controller_left.svg"

        TextInput {
            id: tempoText

            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: tempoLabel.top

            text: "---"
            font.pointSize: 15
            color: "#615f97"
            font.bold: true
            font.family: "Lato"
            anchors.bottomMargin: 1
            enabled: false
            validator: IntValidator {
                bottom: 1; top: 999;
            }

            onAccepted: {
                focus = false
                text = text.removeZerosFromBeginning();
                tempo = parseInt(text)
                cpp.player.beatsPerMinute = tempo
            }
        }

        Label {
            id: tempoLabel
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 4.5

            text: "TEMPO"
            color: "#8790d0"
            font.family: "Lato"
            font.pointSize: 8
        }
    }

    RoundedRect {
        anchors.left: left.right
        anchors.leftMargin: 1
        width: 51.75
        rightRadius: cpp.player.isRecording ? 7.5 : 0
        leftRadius: 0
        height: parent.height

        Text {
            id: name
            text: tempoFactorText
            font.family: "Lato"
            font.bold: true
            color: "#615f97"
            font.pointSize: 11
            anchors.centerIn: parent
        }
    }

    ImageButton {
        width: cpp.player.isRecording ? 0 : 18.75
        height: 18.75
        anchors.right: parent.right
        anchors.top: parent.top
        idleImage: "images/tonality_up.svg"
        hoverImage: "images/tonality_up_pressed.svg"

        onClicked: {
            tempoFactorIncrement++
        }
    }

    ImageButton {
        width: cpp.player.isRecording ? 0 : 18.75
        height: 18.75
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        idleImage: "images/tonality_down.svg"
        hoverImage: "images/tonality_down_pressed.svg"

        onClicked: {
            tempoFactorIncrement--
        }
    }
}
