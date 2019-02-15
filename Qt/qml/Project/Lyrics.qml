import QtQuick 2.0
import "../"

Rectangle {
    id: lyrics
    height: 53.25

    HorizontalLine {
        width: parent.width
        anchors.top: parent.top
        opacity: 0.6
    }

    Text {
        anchors.centerIn: parent
        font.pointSize: 19
        font.bold: true
        font.family: "Lato"
        color: "#24232d"
        text: lyricsText || ""
    }
}
