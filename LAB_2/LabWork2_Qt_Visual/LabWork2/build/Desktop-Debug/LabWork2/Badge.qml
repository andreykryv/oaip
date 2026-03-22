// Badge.qml
import QtQuick 2.15

Rectangle {
    id: root
    property string badgeText: ""
    property string badgeColor: "blue"

    implicitWidth: lbl.implicitWidth + 14
    implicitHeight: 20
    radius: 10

    color: {
        if (badgeColor === "green")  return "#e8f5e9"
        if (badgeColor === "red")    return "#fdecea"
        if (badgeColor === "yellow") return "#fff8e1"
        if (badgeColor === "gray")   return "#f0f4fa"
        if (badgeColor === "teal")   return "#e0f4f8"
        return "#e0f4f8"
    }
    border.width: 1
    border.color: {
        if (badgeColor === "green")  return "#66bb6a"
        if (badgeColor === "red")    return "#ef5350"
        if (badgeColor === "yellow") return "#ffa726"
        if (badgeColor === "gray")   return "#c8d4e8"
        return "#0097b2"
    }

    Text {
        id: lbl
        anchors.centerIn: parent
        text: root.badgeText
        font.family: "Arial"
        font.pixelSize: 10
        font.weight: Font.Bold
        color: {
            if (badgeColor === "green")  return "#2e7d32"
            if (badgeColor === "red")    return "#c62828"
            if (badgeColor === "yellow") return "#e65100"
            if (badgeColor === "gray")   return "#5a7fa8"
            return "#006f87"
        }
    }
}
