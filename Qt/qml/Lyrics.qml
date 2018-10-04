import QtQuick 2.0

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
            font.pointSize: 18
            font.bold: true
            font.family: "LatoBold"
            color: "#24232d"
            text: "Yesterday  All my troubles seemed so far away   Now it looks as though they're here to stay   Oh, I believe in yesterday"
        }
    }
