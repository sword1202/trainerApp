import QtQuick 2.11
import QtQuick.Window 2.11
import QtGraphicalEffects 1.0
import PitchInputReader 1.0
import Player 1.0

Rectangle {
    id: window
    width: cpp.windowWidth
    height: cpp.windowHeight
    focus: true

    Keys.onUpPressed: {
        verticalScroll.upScroller.start()
    }

    Keys.onDownPressed: {
        verticalScroll.downScroller.start()
    }

    Keys.onLeftPressed: {
        horizontalScroll.leftScroller.start()
    }

    Keys.onRightPressed: {
        horizontalScroll.rightScroller.start()
    }

    Keys.onReleased: {
        if (event.key === Qt.Key_Up) {
            verticalScroll.upScroller.stop()
        } else if (event.key === Qt.Key_Down) {
            verticalScroll.downScroller.stop()
        } else if (event.key === Qt.Key_Left) {
            horizontalScroll.leftScroller.stop()
        } else if (event.key === Qt.Key_Right) {
            horizontalScroll.rightScroller.stop()
        }
    }

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

    PitchInputReader {
        id: pitchInputReader

        Component.onCompleted: {
            //start()
        }

        onPitchDetected: {

        }
    }

    Player {
        source: "/Users/semyon/Documents/torero.mvx"

        Component.onCompleted: {
            play()
        }
    }

    SingingProcessManager {
        id: singingProcessManager
    }

    Header {
        id: header
        playHead2: playHead2
    }

    Rectangle {
        id: pianoContainer
        width: 63
        anchors.top: subheader.bottom
        anchors.left: parent.left
        anchors.bottom: horizontalScroll.top

        Piano {
            id: piano
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

    Rectangle {
        anchors.bottom: yardstick.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: header.bottom

        Text {
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 22.5
            text: "The Beatles - Yesterday"
            color: "#24232D"
            font.family: "LatoBold"
            font.bold: true
            font.pointSize: 13.5
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
        piano: piano
        pitchInputReader: pitchInputReader
        tempo: header.tempo
        verticalScroll: verticalScroll
        horizontalScroll: horizontalScroll
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

    TracksContainer {
        id: tracksContainer
        volumeContainerWidth: pianoContainer.width
        visible: header.tracksShowButton.state === "on"
        height: header.tracksShowButton.state === "on" ? 100 : 0
        anchors.right: verticalScroll.left
        anchors.left: parent.left
        anchors.bottom: pianoContainer.bottom
    }

    VerticalScrollBar {
        id: verticalScroll
        height: workspace.height
        pageSize: 0.3
        anchors.top: workspace.top
        anchors.right: parent.right
    }

    PlayHeadTime {
        anchors.bottom: playHeadTriangle.top
        anchors.horizontalCenter: playHeadTriangle.horizontalCenter
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

    Rectangle {
        property real leftOffset: zoom.getIntervalWidth() * 8

        id: playHead2
        color: "#24232d"
        width: 1
        anchors.left: workspace.left
        anchors.bottom: workspace.bottom
        anchors.top: workspace.top
        anchors.leftMargin: leftOffset
        height: workspace.height

        states: [
            State {
                name: "on"
                PropertyChanges {
                    target: playHead2
                    visible: true
                }

                PropertyChanges {
                    target: playHeadTriangle2
                    visible: true
                }
            },
            State {
                name: "off"
                PropertyChanges {
                    target: playHead2
                    visible: false
                }

                PropertyChanges {
                    target: playHeadTriangle2
                    visible: false
                }
            }
        ]
    }

    SvgImage {
        id: playHeadTriangle2
        width: 11.25
        height: 11.25
        anchors.verticalCenter: playHead2.top
        anchors.horizontalCenter: playHead2.horizontalCenter

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
        height: header.lyricsShowButton.state === "on" ? 53.25 : 0
        visible: header.lyricsShowButton.state === "on"

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
}
