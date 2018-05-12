import QtQuick 2.0

Canvas {
    readonly property real baseWidth: 1374.0
    readonly property var zoomMap: [12*4, 10*4, 8*4, 6*4];
    readonly property string gridColor: "#338B89B6"
    readonly property real verticalToHoriznotalGridIntervalRelation: 2.4117701323665077

    property int zoom: 0

    function drawGrid(ctx) {
        var linesCount = zoomMap[zoom];
        var baseIntervalsCount = linesCount + 1;
        var intervalWidth = baseWidth / baseIntervalsCount;

        // draw beats
        for (var x = intervalWidth; x < width; x += intervalWidth) {
            ctx.beginPath();
            ctx.moveTo(x, 0);
            ctx.lineTo(x, height);
            ctx.strokeStyle = gridColor;
            ctx.stroke();
        }

        // draw pitch horizontal lines
        var intervalHeight = intervalWidth / verticalToHoriznotalGridIntervalRelation;
        for (var y = intervalHeight; y < height; y += intervalHeight) {
            ctx.beginPath();
            ctx.moveTo(0, y);
            ctx.lineTo(width, y);
            ctx.strokeStyle = gridColor;
            ctx.stroke();
        }
    }

    onPaint: {
        var ctx = getContext("2d")
        drawGrid(ctx)
    }
}
