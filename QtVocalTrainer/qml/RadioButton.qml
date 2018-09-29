import QtQuick 2.0

Rectangle {
    property bool on: false
    width: 15
    height: 15
    radius: width * 0.5
    color: "white"

    border {
        color: on ? "#24232D" : "#4d8B89B6"
        width: on ? 3 : 1
    }
}
