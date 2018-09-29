import QtQuick 2.0
import QtGraphicalEffects 1.0

Rectangle {
    property int microphoneLevel: 5
    property int selectedMicrophoneIndex: 0

    property string microphoneLevelColor: "#31DD6C"
    property string microphoneLevelBackground: "#DBDCE5"

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
        id: list
        model: names || 0
        anchors.top: header.bottom
        anchors.bottom: footer.top
        height: parent.height
        width: parent.width

        delegate: Item {
            height: 80
            width: parent.width

            Rectangle {
                id: rect
                height: 60
                anchors.left: parent.left
                anchors.leftMargin: 20
                anchors.top: parent.top
                anchors.topMargin: 20
                color: "white"
                anchors.right: parent.right
                anchors.rightMargin: 20

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        selectedMicrophoneIndex = index
                    }
                }

                RadioButton {
                    id: radioButton
                    on: selectedMicrophoneIndex === index
                    anchors.left: parent.left
                    anchors.leftMargin: 30
                    anchors.top: parent.top
                    anchors.topMargin: 13
                }

                Text {
                    id: name
                    anchors.left: radioButton.right
                    anchors.leftMargin: 20
                    anchors.verticalCenter: radioButton.verticalCenter
                    text: modelData
                    font.family: "Lato"
                    color: "#514E64"
                    font.pointSize: 12
                }

                Row {
                    id: row
                    property int level: index === selectedMicrophoneIndex ? microphoneLevel : 0

                    height: 7
                    anchors.left: name.left
                    anchors.top: name.bottom
                    anchors.topMargin: 10
                    spacing: 1

                    RoundedRect {
                        color: row.level > 0 ? microphoneLevelColor : microphoneLevelBackground
                        height: parent.height
                        width: 26
                        leftRadius: height / 2
                    }

                    Repeater {
                        height: parent.height
                        model: 8

                        Rectangle {
                            color: row.level >= 2 + index ? microphoneLevelColor : microphoneLevelBackground
                            width: 26;
                            height: parent.height
                        }
                    }

                    RoundedRect {
                        color: row.level >= 10 ? microphoneLevelColor : microphoneLevelBackground
                        height: parent.height
                        width: 26
                        rightRadius: height / 2
                    }
                }
            }

            DropShadow {
                anchors.fill: rect
                horizontalOffset: 0
                verticalOffset: 0
                radius: 15.0
                samples: 30
                color: "#DDDBEE"
                source: rect
            }
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

            onClicked: {
                self.choose(selectedMicrophoneIndex)
            }
        }

        TextButton {
            text: "Cancel"
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: choose.left
            anchors.rightMargin: 7.5

            onClicked: {
                self.close()
            }
        }
    }
}
