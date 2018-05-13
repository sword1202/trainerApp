import QtQuick 2.0
import "js/canvasutils.js" as CanvasUtils

Canvas {
    width: 51

    readonly property real bigPitchHeight: 25.5
    readonly property real smallPitchHeight: 18.75
    readonly property real distanceBetweenPitches: 2
    readonly property var heightMap: [smallPitchHeight, bigPitchHeight,
        smallPitchHeight, smallPitchHeight, bigPitchHeight, bigPitchHeight, smallPitchHeight]
    readonly property real pitchRadius: 3
    readonly property string borderColor: "#9A98D0"

    property int startPitchIndex: 0

    onPaint: {
        var ctx = getContext("2d")

        ctx.strokeStyle = borderColor
        var index = 0;
        var y = height
        var heightMapLength = heightMap.length
        while (y > -bigPitchHeight) {
            var pitchHeight = heightMap[index % heightMapLength]
            CanvasUtils.roundRect(ctx, 0, y - pitchHeight, width - 1, pitchHeight, {
                tr: pitchRadius,
                br: pitchRadius
            })
            console.log("y = " + y)
            console.log("height = " + pitchHeight)
            y -= pitchHeight
            y -= distanceBetweenPitches
            index++
        }
    }
}
