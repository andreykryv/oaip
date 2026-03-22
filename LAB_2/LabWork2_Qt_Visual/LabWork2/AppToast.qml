// AppToast.qml
// Shows a temporary error or info popup at the bottom of the window.
// Usage:
//   AppToast { id: toast }
//   ...
//   toast.show("Некорректная дата!", "error")   // or "info"

import QtQuick 2.15
import QtQuick.Layouts 1.15

Rectangle {
    id: root
    anchors.horizontalCenter: parent.horizontalCenter
    anchors.bottom: parent.bottom
    anchors.bottomMargin: 24
    width: msgText.implicitWidth + 48
    height: 40
    radius: 5
    opacity: 0
    z: 999

    property string message: ""
    property string type: "error"   // "error" | "info" | "success"

    color: {
        if (type === "error")   return "#3a1a1a"
        if (type === "success") return "#1a3a1a"
        return "#1a2a3a"
    }
    border.color: {
        if (type === "error")   return "#f7546466"
        if (type === "success") return "#57c96e66"
        return "#4a9eff66"
    }
    border.width: 1

    function show(msg, t) {
        message = msg
        type = t || "error"
        showAnim.restart()
    }

    RowLayout {
        anchors.fill: parent
        anchors.leftMargin: 16
        anchors.rightMargin: 16
        spacing: 8

        Text {
            text: root.type === "error" ? "⚠" : root.type === "success" ? "✓" : "ℹ"
            color: root.type === "error" ? "#f75464" : root.type === "success" ? "#57c96e" : "#4a9eff"
            font.pixelSize: 14
        }
        Text {
            id: msgText
            text: root.message
            color: "#dfe1e5"
            font.pixelSize: 12
            font.family: "Consolas"
        }
    }

    SequentialAnimation {
        id: showAnim
        NumberAnimation { target: root; property: "opacity"; to: 1;   duration: 200 }
        PauseAnimation  { duration: 2800 }
        NumberAnimation { target: root; property: "opacity"; to: 0;   duration: 400 }
    }
}
