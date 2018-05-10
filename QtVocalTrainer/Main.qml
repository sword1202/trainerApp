import QtQuick 2.11
import QtQuick.Window 2.11

Rectangle {
    id: window
    width: cpp.windowWidth
    height: cpp.windowHeight

    Rectangle {
        id: header
        height: cpp.dpY(1.041)
        width: parent.width
        color: "#c5cef8"
    }
}
