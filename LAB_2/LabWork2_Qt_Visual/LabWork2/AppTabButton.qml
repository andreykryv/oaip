// AppTabButton.qml
import QtQuick 2.15
import QtQuick.Controls 2.15

TabButton {
    id: root
    property string label: ""
    property bool isActive: false

    implicitWidth: Math.max(140, contentItem.implicitWidth + 32)
    implicitHeight: 40
    text: label
    focusPolicy: Qt.NoFocus

    contentItem: Text {
        text: root.label
        color: root.isActive ? "#ffffff" : (root.hovered ? "#ffffff" : "#a8c0dc")
        font.pixelSize: 13
        font.family: "Arial"
        font.weight: root.isActive ? Font.Bold : Font.Normal
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        Behavior on color { ColorAnimation { duration: 100 } }
    }

    background: Rectangle {
        color: root.isActive ? "#0097b2" : (root.hovered ? "#1e3a70" : "transparent")
        Behavior on color { ColorAnimation { duration: 100 } }

        // Teal underline on active tab
        Rectangle {
            anchors.bottom: parent.bottom
            width: parent.width
            height: root.isActive ? 3 : 0
            color: "#00bcd4"
        }
    }
}
