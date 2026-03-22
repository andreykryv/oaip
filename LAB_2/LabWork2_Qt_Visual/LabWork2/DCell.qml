// DCell.qml
import QtQuick 2.15

Item {
    property string cellText: ""
    property color  cellColor: "#1a2f5e"
    property bool   bold: false
    height: 38

    Rectangle {
        anchors.right: parent.right
        width: 1; height: parent.height
        color: "#dce6f0"
    }
    Text {
        anchors.fill: parent
        anchors.leftMargin: 10
        text: cellText
        color: cellColor
        font.pixelSize: 13; font.family: "Arial"; font.weight: bold ? Font.Bold : Font.Normal
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }
}
