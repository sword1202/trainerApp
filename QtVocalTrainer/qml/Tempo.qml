import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import "js/strings.js" as Strings

Item {
    height: 37.5
    width: 52.5

    ButtonShadow {
        target: svg
    }

    SvgImage {
        id: svg
        anchors.fill: parent

        source: "images/tempo_background.svg"
    }

    TextInput {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: tempoLabel.top

        text: "150"
        font.pointSize: 15
        color: "#615f97"
        font.bold: true
        font.family: "LatoBold"
        anchors.bottomMargin: 1
        validator: IntValidator {
            bottom: 1; top: 999;
        }

        onAccepted: {
            focus = false
            text = text.removeZerosFromBeginning();
        }
    }

    Label {
        id: tempoLabel
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 4.5

        text: "TEMPO"
        color: "#8790d0"
        font.family: "LatoRegular"
        font.pointSize: 7.5
    }
}
