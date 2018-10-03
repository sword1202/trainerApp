function iterateIntervals(target, args) {
    var intervalWidth = cpp.zoomController.intervalWidth;

    var horizontalOffset = args.horizontalOffset || 0
    // draw beats and tacts
    for (var x = intervalWidth - horizontalOffset, index = 1; x < target.width + horizontalOffset; x += intervalWidth, index++) {
        args.beatsIterator(x, index % 4 != 0);
    }

    if (args.verticalIterator) {
        var verticalOffset = args.verticalOffset || 0
        var intervalHeight = cpp.zoomController.intervalHeight
        index = 1;
        for (var y = intervalHeight  - verticalOffset; y < target.height + verticalOffset; y += intervalHeight, index++) {
            args.verticalIterator(target.height - y, index % 12 == 0);
        }
    }
}
