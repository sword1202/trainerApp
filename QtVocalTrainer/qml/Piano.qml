import QtQuick 2.0
import "js/canvasutils.js" as CanvasUtils

Canvas {
    width: 51

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
    readonly property real diestanceBetweenTextLeftAndPitchRight: 1062
    readonly property string borderColor: "#9A98D0"
    readonly property string sharpPitchColor: "#9A98D0"

    property int startPitchIndex: 0

    onPaint: {
        var ctx = getContext("2d")

        ctx.strokeStyle = borderColor
        ctx.fillStyle = sharpPitchColor
        var index = 0;
        var y = height
        var heightMapLength = heightMap.length
        while (y > -bigPitchHeight) {
            var pitchHeight = heightMap[index % heightMapLength]
            CanvasUtils.roundRect(ctx, 0, y - pitchHeight, width - 1, pitchHeight, {
                tr: pitchRadius,
                br: pitchRadius
            })

            y -= pitchHeight

            // draw sharp pitch
            if (hasSharpMap[index % heightMapLength]) {
                CanvasUtils.roundRect(ctx, 0,
                                      y - sharpPitchHeight / 2 - distanceBetweenPitches,
                                      sharpPitchWidth, sharpPitchHeight, {
                    tr: sharpPitchRadius,
                    br: sharpPitchRadius
                }, true, false)

            }

            y -= distanceBetweenPitches
            index++
        }
    }

    Component {
        id: pitchNameText

        Text {
            opacity: 0.9
        }
    }
}