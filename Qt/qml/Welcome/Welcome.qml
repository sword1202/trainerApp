import QtQuick 2.0
import "../"

Item {
    Image {
        anchors.left: parent.left
        height: parent.height
        width: 225
        source: "welcome_gradient.png"

        SideBarItem {
            anchors.topMargin: 21
            anchors.top: parent.top

            title: "Recent Projects"
            selected: true
            hoverIcon: "recent_projects_selected.svg"
        }
    }
}
