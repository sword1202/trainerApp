import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.11

Rectangle {
    width: 800
    height: 500

    ColumnLayout {
        RowLayout {
            Button {
                text: "Select midi file"

                onClicked: {
                    midiFilePath.text = self.showSelectMidi()
                }
            }

            Text {
                id: midiFilePath
            }
        }

        RowLayout {
            Text {
                text: "Enter midi track index"
            }

            TextEdit {
                id: midiIndex
                text: "0"
                inputMethodHints: Qt.ImhDigitsOnly
            }
        }

        RowLayout {
            Button {
                text: "Select instrumental file"

                onClicked: {
                    inctrumentalFilePath.text = self.showSelectInstrumental()
                }
            }

            Text {
                id: inctrumentalFilePath
            }
        }

        Button {
            text: "Submit"

            onClicked: {
                self.submit(inctrumentalFilePath.text, midiFilePath.text, parseInt(midiIndex.text))
            }
        }
    }
}
