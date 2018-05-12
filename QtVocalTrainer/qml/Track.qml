import QtQuick 2.0

Rectangle {
    id: root

    readonly property real imageWidth: 92.25
    readonly property real imageHeight: 27.8025

    property var addedImages: []

    color: "#00000000"

    height: imageHeight

    Component {
        id: factory
        Image {
            height: imageHeight
            width: imageWidth

            source: "images/track_piece.png"
        }
    }

    onWidthChanged: {
        addedImages.forEach(function(img) {
            img.destroy()
        })
        addedImages = []

        for (var x = 0; x < width + imageWidth; x += imageWidth) {
            var obj = factory.createObject(root, {})
            obj.x = x
            obj.y = 0
            addedImages.push(obj)
        }
    }
}
