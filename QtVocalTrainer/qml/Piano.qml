import QtQuick 2.0
import "js/canvasutils.js" as CanvasUtils
import "js/uiutils.js" as UiUtils

Rectangle {
    color: "white"

    VerticalLine {
        anchors.right: parent.right
        height: parent.height
        anchors.top: parent.top
    }

    Canvas {
        id: root

        width: 51

        anchors.left: parent.left
        anchors.top: parent.top
        height: parent.height

        property var firstPitch: cpp.zoomController.firstPitch

        readonly property real bigPitchHeight: 25.5
        readonly property real smallPitchHeight: 18.75
        readonly property real sharpPitchHeight: 9.75
        readonly property real sharpPitchWidth: 22.5
        readonly property real distanceBetweenPitches: 2
        readonly property var heightMap: [smallPitchHeight, bigPitchHeight,
            smallPitchHeight, smallPitchHeight, bigPitchHeight, bigPitchHeight, smallPitchHeight]
        readonly property var hasSharpMap: [true, true, false, true, true, true, false]
        readonly property real pitchRadius: 3
        readonly property real sharpPitchRadius: 1.5
        readonly property real distanceBetweenTextLeftAndPitchRight: 21.0
        readonly property string borderColor: "#7f9A98D0"
        readonly property string sharpPitchColor: "#9A98D0"
        readonly property string reachedPitchColor: "#31DD6C"
        readonly property string missedPitchColor: "#FF5E85"
        readonly property string selectedPitchColor: "#615F97"
        readonly property var pitchNames: ["C", "D", "E", "F", "G", "A", "B"]
        readonly property real intervalOctvaHeightToPianoOctaveHeightRelation: (function() {
            // 12 pitches in octave
            var intervalOctaveHeight = cpp.zoomController.intervalHeight * 12
            // 4 small pitches and 3 big pitches from heightMap, 5: 7 pitches in octave
            var pianoOctaveHeight = smallPitchHeight * 4 + bigPitchHeight * 3 + distanceBetweenPitches * 7
            return intervalOctaveHeight / pianoOctaveHeight
        })();

        property int startPitchIndex: 0

        onPaint: {
            var ctx = getContext("2d")

            ctx.fillStyle = "white";
            ctx.fillRect(0, 0, width, height);

            ctx.strokeStyle = borderColor
            ctx.fillStyle = sharpPitchColor
            var index = firstPitch.whiteIndex
            var perfectFrequencyIndex = firstPitch.perfectFrequencyIndex
            var y = height
            var heightMapLength = heightMap.length

            var detectedPitch = cpp.pitchInputReader.lastDetectedPitch
            var detectedPitchIndex = detectedPitch.perfectFrequencyIndex
            console.log("detectedPitch = " + cpp.pitchFromPerfectFrequencyIndex(detectedPitchIndex).name)

            while (y > -bigPitchHeight) {

                var stroke = true
                var fill = false

                var pitchHeight = heightMap[index % heightMapLength] * intervalOctvaHeightToPianoOctaveHeightRelation
                CanvasUtils.roundRect(ctx, 0, y - pitchHeight, width - 1, pitchHeight, {
                    tr: pitchRadius * intervalOctvaHeightToPianoOctaveHeightRelation,
                    br: pitchRadius * intervalOctvaHeightToPianoOctaveHeightRelation
                }, fill, stroke)

                y -= pitchHeight

                // draw sharp pitch
                pitchHeight = sharpPitchHeight * intervalOctvaHeightToPianoOctaveHeightRelation
                if (hasSharpMap[index % heightMapLength]) {
                    perfectFrequencyIndex++
                    if (detectedPitchIndex === perfectFrequencyIndex) {
                        if (cpp.player.hasPitchNow(detectedPitchIndex)) {
                            ctx.fillStyle = reachedPitchColor
                        } else {
                            ctx.fillStyle = selectedPitchColor
                        }
                    } else {
                        if (cpp.player.hasPitchNow(perfectFrequencyIndex) &&
                                !cpp.player.hasPitchNow(detectedPitchIndex)) {
                            ctx.fillStyle = missedPitchColor
                        } else {
                            ctx.fillStyle = sharpPitchColor
                        }
                    }

                    CanvasUtils.roundRect(ctx, 0,
                                          y - pitchHeight / 2 - distanceBetweenPitches / 2 * intervalOctvaHeightToPianoOctaveHeightRelation,
                                          sharpPitchWidth, pitchHeight, {
                        tr: sharpPitchRadius * intervalOctvaHeightToPianoOctaveHeightRelation,
                        br: sharpPitchRadius * intervalOctvaHeightToPianoOctaveHeightRelation
                    }, true, false)
                }

                y -= distanceBetweenPitches * intervalOctvaHeightToPianoOctaveHeightRelation
                index++
                perfectFrequencyIndex++
            }
        }

        Component {
            id: pitchNameText

            Text {
                opacity: 0.9
                anchors.left: parent.right
                anchors.leftMargin: -root.distanceBetweenTextLeftAndPitchRight
                color: "#24232D"
                font.family: "LatoBold"
                font.bold: true
                font.pixelSize: 9
            }
        }

        function updatePitchNames() {
            UiUtils.destroyAllChildren(root)

            var index = firstPitch.whiteIndex
            var y = height
            var heightMapLength = heightMap.length
            while (y > -bigPitchHeight) {
                var indexInMap = index % heightMapLength
                var pitchHeight = heightMap[indexInMap] * intervalOctvaHeightToPianoOctaveHeightRelation
                var text = pitchNameText.createObject(root, {})
                text.y =  y - pitchHeight / 2 - text.height / 2
                text.text = pitchNames[indexInMap] + (Math.floor(index / heightMapLength) + firstPitch.octave)
                index++
                y -= pitchHeight + distanceBetweenPitches * intervalOctvaHeightToPianoOctaveHeightRelation
            }
        }

        Component.onCompleted: {
            cpp.pitchInputReader.pitchDetected.connect(requestPaint)
        }

        onHeightChanged: {
            updatePitchNames()
        }

        onFirstPitchChanged: {
            updatePitchNames()
            requestPaint()
        }

        onIntervalOctvaHeightToPianoOctaveHeightRelationChanged: {
            updatePitchNames()
            requestPaint()
        }
    }
}
