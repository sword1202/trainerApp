import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.0


ApplicationWindow {
    id: window

    FileDialog {
        id: fileDialog
        title: "Please choose a file"
        folder: shortcuts.home
        selectFolder: false
        selectMultiple: false
        nameFilters: [ "Mvx files (*.mvx)", "All files (*)" ]
        onAccepted: {
            main.openFile(fileUrl)
        }
    }

    menuBar: MenuBar {
        Menu {
            title: "File"
            MenuItem {
                text: "Open..."

                onTriggered: {
                    fileDialog.open()
                }
            }
        }
    }

    Component.onCompleted: {
        window.showMaximized();
    }

    Main {
        id: main
        anchors.fill: parent
    }
}
