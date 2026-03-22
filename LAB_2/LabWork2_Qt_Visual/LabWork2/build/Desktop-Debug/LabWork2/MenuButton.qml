// MenuButton.qml
import QtQuick 2.15

Rectangle {
    property string label: ""
    implicitWidth: t.implicitWidth + 20
    implicitHeight: 38
    color: hov.hovered ? "#1e3a70" : "transparent"
    radius: 3
    Behavior on color { ColorAnimation { duration: 100 } }

    Text {
        id: t
        anchors.centerIn: parent
        text: label
        color: "#ffffff"
        font.pixelSize: 13
        font.family: "Arial"
    }
    HoverHandler { id: hov }
}
