// AppComboBox.qml
import QtQuick 2.15
import QtQuick.Controls 2.15

ComboBox {
    id: root
    implicitHeight: 32
    font.family: "Arial"
    font.pixelSize: 13

    contentItem: Text {
        leftPadding: 10
        rightPadding: root.indicator.width + 8
        text: root.displayText
        color: "#1a2f5e"
        font: root.font
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    background: Rectangle {
        color: "#ffffff"
        border.color: root.popup.visible ? "#0097b2" : "#c8d4e8"
        border.width: root.popup.visible ? 2 : 1
        radius: 4
    }

    indicator: Text {
        x: root.width - width - 10
        y: (root.height - height) / 2
        text: "\u25BE"
        color: "#5a7fa8"
        font.pixelSize: 14
    }

    delegate: ItemDelegate {
        width: root.width
        height: 30
        highlighted: root.highlightedIndex === index
        contentItem: Text {
            text: modelData
            color: root.currentIndex === index ? "#0097b2" : "#1a2f5e"
            font.family: "Arial"
            font.pixelSize: 13
            verticalAlignment: Text.AlignVCenter
            leftPadding: 10
        }
        background: Rectangle {
            color: parent.highlighted ? "#f0f4fa" : "#ffffff"
        }
    }

    popup: Popup {
        y: root.height + 2
        width: root.width
        padding: 0
        background: Rectangle {
            color: "#ffffff"
            border.color: "#c8d4e8"
            border.width: 1
            radius: 4
            layer.enabled: true
            layer.effect: null
        }
        contentItem: ListView {
            implicitHeight: Math.min(contentHeight, 200)
            model: root.delegateModel
            clip: true
            ScrollIndicator.vertical: ScrollIndicator {}
        }
    }
}
