import QtQuick 2.0

Item {
    height: 37.5
    width: 52.5

    SvgImage {
        id: svg
        anchors.fill: parent

        source: "images/tempo_background.svg"
    }

    ButtonShadow {
        target: svg
    }
}
