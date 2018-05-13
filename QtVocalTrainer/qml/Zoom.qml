import QtQuick 2.0

Item {
    readonly property real baseWidth: 1374.0
    readonly property var zoomMap: [12*4, 10*4, 8*4, 6*4];
    readonly property real verticalToHoriznotalGridIntervalRelation: 2.4117701323665077

    property int zoom: 0

    function getIntervalWidth() {
        var linesCount = zoomMap[zoom];
        var baseIntervalsCount = linesCount + 1;
        return baseWidth / baseIntervalsCount;
    }

    function getIntervalHeight() {
        return getIntervalWidth() / verticalToHoriznotalGridIntervalRelation
    }

    function iterateIntervals(target,args) {
        var intervalWidth = getIntervalWidth();

        // draw beats and tacts
        for (var x = intervalWidth, index = 1; x < target.width; x += intervalWidth, index++) {
            args.beatsIterator(x, index % 4 != 0);
        }

        if (args.verticalIterator) {
            var intervalHeight = intervalWidth / verticalToHoriznotalGridIntervalRelation;
            index = 1;
            for (var y = intervalHeight; y < target.height; y += intervalHeight, index++) {
                args.verticalIterator(target.height - y, index % 12 == 0);
            }
        }
    }
}
