// AppErrorDialog.qml
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Popup {
    id: root
    property string message: ""
    property string dlgTitle: "Ошибка"
    property bool   isError: true

    function show(msg) {
        message = msg
        open()
    }

    x: (parent ? parent.width  - width  : 0) / 2
    y: (parent ? parent.height - height : 0) / 2
    modal: true
    focus: true
    width: 360
    padding: 0
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

    background: Rectangle {
        color: "#2b2d30"
        border.color: "#3d3f41"
        border.width: 1
        radius: 4
    }

    Column {
        width: parent.width
        spacing: 0

        Rectangle {
            width: parent.width
            height: 34
            color: "#24252a"
            radius: 4

            Text {
                anchors.fill: parent
                anchors.leftMargin: 14
                text: root.dlgTitle
                color: root.isError ? "#f75464" : "#4a9eff"
                font.pixelSize: 13
                font.family: "Consolas"
                font.bold: true
                verticalAlignment: Text.AlignVCenter
            }
        }

        Text {
            width: parent.width
            padding: 16
            text: root.message
            color: "#bcbec4"
            font.pixelSize: 12
            font.family: "Consolas"
            wrapMode: Text.Wrap
        }

        Item {
            width: parent.width
            height: 44

            AppButton {
                anchors.right: parent.right
                anchors.rightMargin: 12
                anchors.verticalCenter: parent.verticalCenter
                text: "OK"
                variant: root.isError ? "danger" : "primary"
                onClicked: root.close()
            }
        }
    }
}
