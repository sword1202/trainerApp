import QtQuick 2.0
import QtGraphicalEffects 1.0
import "../"

Item {
    Image {
        id: sidebar
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: footer.top
        width: 225
        source: "welcome_gradient.png"

        SideBarItem {
            id: recentProjects
            otherItems: [newProject, recentRecordings]
            anchors.topMargin: 21
            anchors.top: parent.top

            title: "Recent Projects"
            selected: true
            normalIcon: "recent_projects.svg"
            hoverIcon: "recent_projects_selected.svg"
        }

        SideBarItem {
            id: newProject
            otherItems: [recentProjects, recentRecordings]
            anchors.top: recentProjects.bottom

            title: "New Project"
            selected: false
            normalIcon: "new_project_sidebar.svg"
            hoverIcon: "new_project_sidebar_selected.svg"
        }

        SideBarItem {
            id: recentRecordings
            otherItems: [newProject, recentProjects]
            anchors.top: newProject.bottom

            title: "Recent Recordings"
            selected: false
            normalIcon: "recent_recordings.svg"
            hoverIcon: "recent_recordings_selected.svg"
        }
    }

    GridView {
        id: list
        anchors.left: sidebar.right
        anchors.top: parent.top
        anchors.bottom: footer.top
        anchors.right: parent.right

        anchors.leftMargin: 4 + spacing
        anchors.rightMargin: 4 + spacing
        anchors.topMargin: 2 + spacing
        anchors.bottomMargin: 2 + spacing

        property real spacing: 7.5
        property real itemWidth: 143
        property real itemHeight: newProject.selected ? 125 : 140

        cellWidth: itemWidth + spacing
        cellHeight: itemHeight + spacing

        model: newProject.selected ? [""] : (recentProjects.selected ? projects : recordings)

        delegate: Rectangle {
            width: list.itemWidth
            height: list.itemHeight

            Rectangle {
                id: border
                visible: mouseArea.containsMouse

                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                height: 98

                border.width: 1
                border.color: "#615F97"
                radius: 3
            }

            function getSvgSource() {
                if (newProject.selected) {
                    return mouseArea.containsMouse ? "new_project_hover.svg" : "new_project.svg"
                } else if(recentProjects.selected) {
                    return mouseArea.containsMouse ? "project_hover.svg" : "project.svg"
                } else {
                    return mouseArea.containsMouse ? "recording_hover.svg" : "recording.svg"
                }
            }

            SvgImage {
                id: image
                source: getSvgSource()
                width: 135
                height: 90
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: 4
                anchors.rightMargin: 4
                anchors.topMargin: 4
                anchors.bottomMargin: 4

                Text {
                    function getScore() {
                        if (model.modelData.score !== undefined) {
                            return model.modelData.score + "%"
                        }

                        return ""
                    }

                    text: getScore()
                    anchors.centerIn: parent
                    font.family: "Lato"
                    font.pointSize: 13
                }
            }

            MouseArea {
                id: mouseArea
                cursorShape: Qt.OpenHandCursor
                anchors.fill: image
                hoverEnabled: true
            }

            Item {
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: border.bottom

                Text {
                    anchors.centerIn: parent
                    text: newProject.selected ? "Empty Project" : model.modelData.artistName + "\n" + model.modelData.title
                    color: mouseArea.containsMouse ? "#615F97" : "#24232D"
                    font.pointSize: 12
                    font.family: "Lato"
                    horizontalAlignment: Text.AlignHCenter
                }
            }
        }
    }

    DropShadow {
        anchors.fill: footer
        source: footer

        color: "#615F97"
        opacity: 0.14
        samples: 20
        radius: 20
    }

    Rectangle {
        id: footer
        height: 67.5
        width: parent.width
        anchors.bottom: parent.bottom

        TextButton {
            text: "Open an existing Project..."
            anchors.leftMargin: 22.5
            anchors.left: parent.left
            width: 172.5
            anchors.verticalCenter: parent.verticalCenter
        }
    }
}
