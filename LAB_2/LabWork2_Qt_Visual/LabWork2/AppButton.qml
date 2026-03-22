// AppButton.qml
import QtQuick 2.15
import QtQuick.Controls 2.15

Button {
    id: root
    property string variant: "default"
    implicitHeight: 32
    leftPadding: 18
    rightPadding: 18
    focusPolicy: Qt.NoFocus

    contentItem: Text {
        text: root.text
        font.pixelSize: 13
        font.family: "Arial"
        font.weight: Font.Medium
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        color: {
            if (root.variant === "primary") return "#ffffff"
            if (root.variant === "danger")  return "#c62828"
            if (root.variant === "ghost")   return "#5a7fa8"
            return "#1a2f5e"
        }
    }

    background: Rectangle {
        radius: 4
        border.width: 1
        border.color: {
            if (root.variant === "primary") return root.hovered ? "#007a94" : "#0097b2"
            if (root.variant === "danger")  return "#ef9a9a"
            if (root.variant === "ghost")   return "#c8d4e8"
            return "#c8d4e8"
        }
        color: {
            if (root.variant === "primary") return root.hovered ? "#007a94" : "#0097b2"
            if (root.variant === "danger")  return root.hovered ? "#fdecea" : "#ffffff"
            if (root.variant === "ghost")   return root.hovered ? "#f0f4fa" : "transparent"
            return root.hovered ? "#e8eef8" : "#f0f4fa"
        }
        Behavior on color { ColorAnimation { duration: 100 } }
    }
}
