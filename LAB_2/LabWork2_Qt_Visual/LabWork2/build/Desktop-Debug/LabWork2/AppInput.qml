// AppInput.qml
import QtQuick 2.15
import QtQuick.Controls 2.15

TextField {
    id: root
    color: "#1a2f5e"
    font.family: "Arial"
    font.pixelSize: 13
    leftPadding: 10
    rightPadding: 10
    implicitHeight: 32
    selectByMouse: true
    placeholderTextColor: "#aab8cc"

    background: Rectangle {
        color: "#ffffff"
        border.color: root.activeFocus ? "#0097b2" : "#c8d4e8"
        border.width: root.activeFocus ? 2 : 1
        radius: 4
        Behavior on border.color { ColorAnimation { duration: 100 } }
    }
}
