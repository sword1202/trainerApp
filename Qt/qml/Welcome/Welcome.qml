import QtQuick 2.0
import "../"

Item {
    Image {
        id: sidebar
        anchors.left: parent.left
        height: parent.height
        width: 225
        source: "welcome_gradient.png"

        SideBarItem {
            id: recentProjects
            anchors.topMargin: 21
            anchors.top: parent.top

            title: "Recent Projects"
            selected: true
            normalIcon: "recent_projects.svg"
            hoverIcon: "recent_projects_selected.svg"
        }

        SideBarItem {
            id: newProject
            anchors.top: recentProjects.bottom

            title: "New Project"
            selected: false
            normalIcon: "new_project_sidebar.svg"
            hoverIcon: "new_project_sidebar_selected.svg"
        }

        SideBarItem {
            anchors.top: newProject.bottom

            title: "Recent Recordings"
            selected: false
            normalIcon: "recent_recordings.svg"
            hoverIcon: "recent_recordings_selected.svg"
        }
    }

    ListView {
        anchors.left: sidebar.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.leftMargin: 19
        anchors.rightMargin: 19
        anchors.topMargin: 17
        anchors.bottomMargin: 17

        spacing: 15

        model: [""]

        delegate: Rectangle {
            width: 143
            height: 125

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

            SvgImage {
                id: image
                source: mouseArea.containsMouse ? "new_project_hover.svg" : "new_project.svg"
                width: 135
                height: 90
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: 4
                anchors.rightMargin: 4
                anchors.topMargin: 4
                anchors.bottomMargin: 4
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
                    text: "Empty Project"
                    color: mouseArea.containsMouse ? "#615F97" : "#24232D"
                    font.pointSize: 12
                    font.family: "Lato"
                }
            }
        }
    }
}
