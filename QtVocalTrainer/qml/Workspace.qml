import QtQuick 2.0

Canvas {
    id: root

    readonly property string gridColor: "#338B89B6"
    readonly property string accentGridColor: "#808B89B6"

    property var zoom: zoom

    function drawGrid(ctx) {
        zoom.iterateIntervals(root, {
            beatsIterator: function(x, isBeat) {
                // draw beats and tacts
                ctx.beginPath();
                ctx.moveTo(x, 0);
                ctx.lineTo(x, height);
                ctx.strokeStyle = isBeat ? gridColor : accentGridColor
                ctx.stroke();
            },
            verticalIterator: function(y, isOctaveBegin) {
                // draw pitch horizontal lines
                ctx.beginPath();
                ctx.moveTo(0, y);
                ctx.lineTo(width, y);
                ctx.strokeStyle = isOctaveBegin ? accentGridColor : gridColor;
                ctx.stroke();
            }
        });
    }

    onPaint: {
        var ctx = getContext("2d")
        drawGrid(ctx)
    }
}
