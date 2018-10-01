import QtQuick 2.0
import QtGraphicalEffects 1.0


Rectangle {
    color: "green"
    width: 200
    height: 200

    Component.onCompleted: {
        console.log("width = " + width + " height = " + height)
    }

    Item {
        width: 225
        height: parent.height

        LinearGradient {
            anchors.fill: parent
            start: Qt.point(0, 0)
            end: Qt.point(parent.width, parent.height)
            gradient: Gradient {
                GradientStop { position: 0.0; color: "#C0D1FF" }
                GradientStop { position: 1.0; color: "#CBC7F9" }
            }
        }
    }
}
