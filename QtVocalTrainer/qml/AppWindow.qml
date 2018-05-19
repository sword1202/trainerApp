import QtQuick 2.0
import QtQuick.Controls 1.4

ApplicationWindow {
    id: window

    Component.onCompleted: {
        window.showMaximized();
    }

    Main {
        anchors.fill: parent
    }
}
