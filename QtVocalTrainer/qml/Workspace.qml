import QtQuick 2.0

Canvas {
    readonly property real baseWidth: 1374.0
    readonly property var zoomMap: [12*4, 10*4, 8*4, 6*4];
    readonly property string gridColor: "#338B89B6"

    property int zoom: 0

    function drawVerticalLines(ctx) {
        var linesCount = zoomMap[zoom];
        var intervalsCount = linesCount + 1;
        var intervalWidth = baseWidth / intervalsCount;

        for (var x = intervalWidth; x < width; x += intervalWidth) {
            ctx.beginPath();
            ctx.moveTo(x, 0);
            ctx.lineTo(x, height);
            ctx.strokeStyle = gridColor;
            ctx.stroke();
        }
    }

    onPaint: {
        var ctx = getContext("2d")
        drawVerticalLines(ctx)
    }
}
