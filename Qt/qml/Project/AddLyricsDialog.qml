import QtQuick 2.0
import "../"

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
            text: "Add lyrics"
            font.family: "Lato"
            font.pointSize: 16
        }

        HorizontalLine {
            opacity: 0.6
            anchors.bottom: parent.bottom
            width: parent.width
        }
    }

    Rectangle {
        color: "white"
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: footer.top
        anchors.top: header.bottom
        anchors.leftMargin: 15
        anchors.rightMargin: 15
        anchors.topMargin: 15
        anchors.bottomMargin: 15
        radius: 3

        TextEdit {
            id: textInput
            anchors.leftMargin: 5
            anchors.rightMargin: 5
            anchors.topMargin: 5
            anchors.bottomMargin: 5
            anchors.fill: parent
            wrapMode: TextEdit.Wrap
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
            id: add
            text: "Add"
            enabled: textInput.text != ""
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 11.25

            onClicked: {
                self.add(textInput.text)
            }
        }

        TextButton {
            text: "Cancel"
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: add.left
            anchors.rightMargin: 7.5

            onClicked: {
                self.close()
            }
        }
    }
}
