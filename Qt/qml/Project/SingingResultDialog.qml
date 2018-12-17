import QtQuick 2.0
import "../"

SvgImage {
    width: 462
    height: 348
    source: "singing_result_dialog_background.svg"

    Text {
        id: header
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 50
        text: "Your score is:"
        color: "#24232D"
        font.family: "Lato"
        font.pointSize: 32
    }

    Item {
        width: parent.width
        anchors.top: header.bottom
        anchors.bottom: footer.top

        Text {
            color: "#31DD6C"
            font.family: "Lato"
            font.pointSize: 80
            text: "87%"
            anchors.centerIn: parent
        }
    }

    Item {
        id: footer
        width: parent.width
        height: 20
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 40
        anchors.rightMargin: 40
        anchors.bottomMargin: 40

        TextOnlyButton {
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            text: "Close"

            onClicked: {
                self.close()
            }
        }

        TextOnlyButton {
            anchors.centerIn: parent
            text: "Save"
        }

        TextOnlyButton {
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            text: "Again"
        }
    }
}
