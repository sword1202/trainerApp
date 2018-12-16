import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import "../../js/strings.js" as Strings
import QtGraphicalEffects 1.0
import "../../"

Item {
    id: root
    property int tempo: cpp.player.source ? cpp.player.beatsPerMinute : -1

    height: 38.25
    width: 52

    Component.onCompleted: {

    }

    onTempoChanged: {
        tempoText.text = tempo > 0 ? tempo.toString() : "---"
    }

    DropShadow {
        anchors.fill: svg
        source: svg
        color: "#9DA3DC"
        opacity: 0.8
        verticalOffset: 1.5
        radius: 3
    }

    SvgImage {
        id: svg
        anchors.fill: parent

        source: "images/tempo_background.svg"
    }

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
