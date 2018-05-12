import QtQuick 2.0
import "js/uiutils.js" as UiUtils

Rectangle {
    id: root

    readonly property real imageWidth: 92.25
    readonly property real imageHeight: 27.8025
    readonly property real smallImageWidth: 90
    readonly property real smallImageHeight: 13.905

    color: "#00000000"

    height: imageHeight

    Rectangle {
        color: "#00000000"
        anchors.fill: parent

        Component {
            id: factory
            Image {
                height: imageHeight
                width: imageWidth

                source: "images/track_piece.png"
            }
        }

        onWidthChanged: {
            UiUtils.destroyAllChildern(this)

            var smallImage = factory.createObject(this, {
                height: smallImageHeight,
                width: smallImageWidth
            })
            smallImage.x = 0;
            smallImage.y = (imageHeight - smallImageHeight) / 2

            for (var x = smallImageWidth; x < width + imageWidth; x += imageWidth) {
                var image = factory.createObject(this, {})
                image.x = x
                image.y = 0
            }
        }
    }

    TrackButton {
        width: 96
        text: "Instrumental track"
    }
}
