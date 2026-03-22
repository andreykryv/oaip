// AppGroupBox.qml
import QtQuick 2.15
import QtQuick.Layouts 1.15

Item {
    id: root
    property string title: ""
    default property alias contentData: contentColumn.data

    implicitWidth:  contentColumn.implicitWidth  + 24
    implicitHeight: contentColumn.implicitHeight + 30

    // Border rectangle
    Rectangle {
        anchors.fill: parent
        anchors.topMargin: 8
        color: "#ffffff"
        border.color: "#dce6f0"
        border.width: 1
        radius: 6
    }

    // Title badge
    Rectangle {
        x: 12; y: 0
        width: titleText.implicitWidth + 12
        height: 18
        color: "#f0f4fa"

        Text {
            id: titleText
            anchors.centerIn: parent
            text: root.title
            color: "#1a2f5e"
            font.pixelSize: 11
            font.family: "Arial"
            font.weight: Font.Bold
        }
    }

    // Content — uses ColumnLayout so children participate in layout sizing
    ColumnLayout {
        id: contentColumn
        anchors.top:    parent.top
        anchors.left:   parent.left
        anchors.right:  parent.right
        anchors.bottom: parent.bottom
        anchors.topMargin:    24
        anchors.leftMargin:   12
        anchors.rightMargin:  12
        anchors.bottomMargin: 10
        spacing: 6
    }
}
