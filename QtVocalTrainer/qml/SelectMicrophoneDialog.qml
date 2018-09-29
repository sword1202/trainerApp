import QtQuick 2.0

Rectangle {
    width: 439
    height: 331
    color: "#F6F5FC"

    Rectangle {
        id: header
        width: parent.width
        height: 45
        color: "white"

        Text {
            anchors.centerIn: parent
            text: "Choose one microphone for singing"
            font.family: "Lato"
            font.pointSize: 16
        }

        HorizontalLine {
            opacity: 0.6
            anchors.bottom: parent.bottom
            width: parent.width
        }
    }

    ListView {
        model: names
        anchors.top: header.bottom
        anchors.bottom: footer.top
        height: parent.height

        delegate: Text {
            text: modelData
        }
    }

    Rectangle {
        id: footer
        height: 45
        width: parent.width
        color: "white"
        anchors.bottom: parent.bottom

        HorizontalLine {
            opacity: 0.6
            anchors.top: parent.top
            width: parent.width
        }

        TextButton {
            id: choose
            text: "Choose"
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 11.25
        }

        TextButton {
            text: "Cancel"
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: choose.left
            anchors.rightMargin: 7.5
        }
    }
}
