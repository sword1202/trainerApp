import QtQuick 2.0
import QtGraphicalEffects 1.0


Rectangle {
    Item {
        width: 225
        height: parent.height

        LinearGradient {
            anchors.fill: parent
            start: Qt.point(-55, -75)
            end: Qt.point(parent.width + 55, parent.height * 0.7)
            gradient: Gradient {
                GradientStop { position: 0.0; color: "#C0D1FF" }
                GradientStop { position: 1.0; color: "#CBC7F9" }
            }
        }
    }
}
