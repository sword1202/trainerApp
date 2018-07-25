import QtQuick 2.0
import "js/canvasutils.js" as CanvasUtils
import "js/uiutils.js" as UiUtils

Rectangle {
    color: "white"

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
        property var selectedWhitePitchIndexes: []

        renderStrategy: Canvas.Threaded

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

            var drawSharpPitchesY = []
            var drawSharpPitchesFillColor = []
            selectedWhitePitchIndexes = []

            while (y > -bigPitchHeight) {

                var stroke = true
                var fill = false

                function getFillColor(perfectFrequencyIndex) {
                    if (detectedPitchIndex === perfectFrequencyIndex) {
                        if (cpp.player.hasPitchNow(detectedPitchIndex)) {
                            return reachedPitchColor
                        } else {
                            return selectedPitchColor
                        }
                    } else {
                        if (cpp.player.hasPitchNow(perfectFrequencyIndex) &&
                                !cpp.player.hasPitchNow(detectedPitchIndex)) {
                            return missedPitchColor
                        } else {
                            return sharpPitchColor
                        }
                    }
                }

                var fillColor = getFillColor(perfectFrequencyIndex)
                if (fillColor !== sharpPitchColor) {
                    stroke = false
                    fill = true
                    ctx.fillStyle = fillColor
                    selectedWhitePitchIndexes.push(index)
                }

                var pitchHeight = heightMap[index % heightMapLength] * intervalOctvaHeightToPianoOctaveHeightRelation
                CanvasUtils.roundRect(ctx, 0, y - pitchHeight, width - 1, pitchHeight, {
                    tr: pitchRadius * intervalOctvaHeightToPianoOctaveHeightRelation,
                    br: pitchRadius * intervalOctvaHeightToPianoOctaveHeightRelation
                }, fill, stroke)

                y -= pitchHeight

                // post draw sharp pitch command
                if (hasSharpMap[index % heightMapLength]) {
                    perfectFrequencyIndex++
                    fillColor = getFillColor(perfectFrequencyIndex)
                    drawSharpPitchesFillColor.push(fillColor)
                    drawSharpPitchesY.push(y)
                }

                y -= distanceBetweenPitches * intervalOctvaHeightToPianoOctaveHeightRelation
                index++
                perfectFrequencyIndex++
            }

            function drawSharpPitch(y, fillColor) {
                ctx.fillStyle = fillColor
                var pitchHeight = sharpPitchHeight * intervalOctvaHeightToPianoOctaveHeightRelation
                CanvasUtils.roundRect(ctx, 0,
                                      y - pitchHeight / 2 - distanceBetweenPitches / 2 * intervalOctvaHeightToPianoOctaveHeightRelation,
                                      sharpPitchWidth, pitchHeight, {
                    tr: sharpPitchRadius * intervalOctvaHeightToPianoOctaveHeightRelation,
                    br: sharpPitchRadius * intervalOctvaHeightToPianoOctaveHeightRelation
                }, true, false)
            }

            for (var i = 0; i < drawSharpPitchesY.length; i++) {
                drawSharpPitch(drawSharpPitchesY[i], drawSharpPitchesFillColor[i])
            }
        }

        onPainted: {
            updatePitchNames()
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

                if (selectedWhitePitchIndexes.indexOf(index) >= 0) {
                    text.color = "white"
                }

                index++
                y -= pitchHeight + distanceBetweenPitches * intervalOctvaHeightToPianoOctaveHeightRelation
            }
        }

        Connections {
            target: cpp.pitchInputReader

            onPitchDetected: {
                //root.requestPaint()
            }
        }

        onHeightChanged: {
            requestPaint()
        }

        onFirstPitchChanged: {
            requestPaint()
        }

        onIntervalOctvaHeightToPianoOctaveHeightRelationChanged: {
            requestPaint()
        }
    }
}
