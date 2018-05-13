import QtQuick 2.11
import QtQuick.Window 2.11
import QtGraphicalEffects 1.0

Rectangle {
    id: window
    width: cpp.windowWidth
    height: cpp.windowHeight

    FontLoader {
        id: latoBold
        name: "LatoBold"
        source: "fonts/Lato-Bold.ttf"
    }

    FontLoader {
        id: latoRegular
        name: "LatoRegular"
        source: "fonts/Lato-Regular.ttf"
    }

    Zoom {
        id: zoom
    }

    Rectangle {
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

    Rectangle {
        id: pianoContainer
        width: 63
        anchors.top: subheader.bottom
        anchors.left: parent.left
        anchors.bottom: horizontalScroll.top

        Piano {
            zoom: zoom
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            anchors.bottomMargin: distanceBetweenPitches * intervalOctvaHeightToPianoOctaveHeightRelation
            anchors.top: parent.top
        }
    }

    Rectangle {
        id: subheader
        height: 59.25
        color: "white"
        width: parent.width
        anchors.left: parent.left
        anchors.top: header.bottom

        ZoomSlider {
            zoom: zoom
            anchors.top: parent.top
            anchors.topMargin: 15
            anchors.rightMargin: 23.25
            anchors.right: parent.right
        }

        HorizontalLine {
            width: parent.width
            anchors.bottom: parent.bottom
            anchors.left: parent.left
        }
    }

    Yardstick {
        id: yardstick
        zoom: zoom
        anchors.bottom: playHeadTriangle.top
        anchors.left: workspace.left
        anchors.right: parent.right
    }

    Workspace {
        id: workspace
        zoom: zoom
        anchors.top: subheader.bottom
        anchors.left: pianoContainer.right
        anchors.leftMargin: 0
        anchors.bottom: horizontalScroll.top
        anchors.right: verticalScroll.right

        MouseArea {
            anchors.fill: parent
            onClicked: {
                playHead.leftOffset = mouse.x
            }
        }
    }

    HorizontalLine {
        width: parent.width
        anchors.bottom: tracksContainer.top
    }

    Rectangle {
        id: tracksContainer
        visible: tracksShowButton.state === "on"
        color: "#00000000"
        height: tracksShowButton.state === "on" ? 111 : 0
        anchors.right: verticalScroll.left
        anchors.left: parent.left
        anchors.bottom: pianoContainer.bottom

        Rectangle {
            id: leftRect
            color: "white"
            height: parent.height
            width: pianoContainer.width
            anchors.left: parent.left
        }

        DropShadow {
            anchors.fill: vocal

            horizontalOffset: 0
            verticalOffset: 0
            radius: 30.0
            color: "#DDDBEE"
            source: vocal
        }

        Rectangle {
            id: vocal
            color: "#D2D0DD"
            anchors.bottom: instrumental.top
            anchors.bottomMargin: 12.75
            height: 30
            anchors.left: leftRect.right
            anchors.right: parent.right

            Rectangle {
                color: "white"
                height: parent.height
                anchors.left: parent.left
                width: 25.5
            }

            TrackButton {
                width: 67.5
                text: "Vocal track"
            }
        }

        Track {
            id: instrumental
            anchors.left: leftRect.right
            anchors.bottom: parent.bottom
            anchors.bottomMargin: horizontalScroll.height + 14.25
            anchors.right: parent.right
        }
    }

    VerticalScrollBar {
        id: verticalScroll
        height: workspace.height
        pageSize: 0.3
        anchors.top: workspace.top
        anchors.right: parent.right
    }

    Rectangle {
        property real leftOffset: zoom.getIntervalWidth() * 4

        id: playHead
        color: "#24232d"
        width: 1
        anchors.left: workspace.left
        anchors.bottom: workspace.bottom
        anchors.top: workspace.top
        anchors.leftMargin: leftOffset
        height: workspace.height
    }

    SvgImage {
        id: playHeadTriangle
        width: 11.25
        height: 11.25
        anchors.verticalCenter: playHead.top
        anchors.horizontalCenter: playHead.horizontalCenter

        source: "images/play_head_triangle.svg"
    }

    HorizontalScrollBar {
        id: horizontalScroll
        pageSize: 0.3
        anchors.bottom: lyrics.top
        anchors.left: pianoContainer.right
        anchors.right: parent.right
    }

    VerticalLine {
        anchors.right: horizontalScroll.left
        anchors.bottom: lyrics.top
        anchors.top: yardstick.top
    }

    Rectangle {
        id: lyrics
        width: parent.width
        anchors.bottom: parent.bottom
        height: lyricsShowButton.state === "on" ? 53.25 : 0
        visible: lyricsShowButton.state === "on"

        HorizontalLine {
            width: parent.width
            anchors.top: parent.top
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
}
