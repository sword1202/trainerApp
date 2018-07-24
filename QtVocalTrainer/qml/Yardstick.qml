import QtQuick 2.0
import "js/zoom.js" as Zoom

Rectangle {
    readonly property real dotRadius: 1.5
    readonly property string dotColor: "#24232D"

    height: 13.5

    Canvas {
        id: canvas
        anchors.fill: parent

        onPaint: {
            var ctx = getContext("2d")

            ctx.fillStyle = "white";
            ctx.fillRect(0, 0, width, height);

            var dotY = height / 2

            var tact = 1;
            Zoom.iterateIntervals(this, {
                beatsIterator: function(x, isBeat) {
                    if (isBeat) {
                        ctx.fillStyle = dotColor;
                        ctx.beginPath();
                        ctx.arc(x, dotY, dotRadius, 0, 2 * Math.PI);
                        ctx.fill();
                    } else {
                        ctx.font = "12px Arial"
                        ctx.fillStyle = dotColor
                        ctx.textAlign = "center"
                        ctx.textBaseline="middle"
                        ctx.fillText((tact - 1).toString(), x, dotY);
                        tact++;
                    }
                }
            })
        }
    }

    Component.onCompleted: {
        cpp.zoomController.zoomChanged.connect(canvas.requestPaint)
    }
}
