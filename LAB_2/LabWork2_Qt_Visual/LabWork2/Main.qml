// Main.qml
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

ApplicationWindow {
    id: root
    visible: true
    width: 1100
    height: 700
    minimumWidth: 900
    minimumHeight: 600
    title: "LabWork2 — БГУИР"
    color: "#f0f4fa"

    // ── БГУИР palette ────────────────────────────────────────
    readonly property color cNavy:    "#1a2f5e"   // primary dark blue
    readonly property color cAccent:  "#0097b2"   // teal accent
    readonly property color cBg:      "#f0f4fa"   // page background
    readonly property color cWhite:   "#ffffff"
    readonly property color cBorder:  "#dce6f0"
    readonly property color cTextHi:  "#ffffff"
    readonly property color cTextDim: "#a8c0dc"
    readonly property color cText:    "#1a2f5e"
    readonly property color cGreen:   "#2e7d32"

    // ── Header (БГУИР navbar) ────────────────────────────────
    header: Rectangle {
        width: parent.width
        height: 56
        color: cNavy

        // Thin red line at very top
        Rectangle {
            anchors.top: parent.top
            width: parent.width; height: 3
            color: cAccent
        }

        RowLayout {
            anchors.fill: parent
            anchors.leftMargin: 16
            anchors.rightMargin: 16
            anchors.topMargin: 3
            spacing: 0

            // Logo block
            Row {
                spacing: 10
                // Shield icon placeholder
                Rectangle {
                    width: 34; height: 34
                    radius: 4
                    color: cAccent
                    anchors.verticalCenter: parent.verticalCenter

                    Column {
                        anchors.centerIn: parent
                        spacing: 2
                        Text { text: "БГ";  color: "white"; font.pixelSize: 10; font.weight: Font.Bold; font.family: "Arial"; anchors.horizontalCenter: parent.horizontalCenter }
                        Text { text: "УИР"; color: "white"; font.pixelSize: 9;  font.weight: Font.Bold; font.family: "Arial"; anchors.horizontalCenter: parent.horizontalCenter }
                    }
                }
                Column {
                    anchors.verticalCenter: parent.verticalCenter
                    spacing: 1
                    Text {
                        text: "БГУИР"
                        color: "#ffffff"
                        font.pixelSize: 15
                        font.weight: Font.Bold
                        font.family: "Arial"
                    }
                    Text {
                        text: "Лабораторная работа №2"
                        color: cTextDim
                        font.pixelSize: 10
                        font.family: "Arial"
                    }
                }
            }

            Item { Layout.fillWidth: true }

            // Nav items
            Repeater {
                model: ["Файл", "Правка", "Вид", "Справка"]
                delegate: MenuButton { label: modelData }
            }

            Item { width: 16 }

            // Date badge
            Rectangle {
                height: 26; width: dateLabel.implicitWidth + 16
                radius: 13; color: "#ffffff20"
                anchors.verticalCenter: parent.verticalCenter
                Text {
                    id: dateLabel
                    anchors.centerIn: parent
                    text: Qt.formatDate(new Date(), "dd.MM.yyyy")
                    color: "#ffffff"; font.pixelSize: 12; font.family: "Arial"
                }
            }
        }
    }

    // ── Footer / status bar ──────────────────────────────────
    footer: Rectangle {
        width: parent.width; height: 30
        color: cNavy

        Rectangle { anchors.top: parent.top; width: parent.width; height: 2; color: cAccent }

        RowLayout {
            anchors.fill: parent
            anchors.leftMargin: 16; anchors.rightMargin: 16
            spacing: 0

            StatusItem {
                label: "Задание"
                value: (tabBar.currentIndex + 1) + " — " +
                       ["Даты","Деканат","Выражения","Типы"][tabBar.currentIndex]
            }
            StatusSep {}
            StatusItem { label: "Qt"; value: "6 | C++17" }
            StatusSep {}
            StatusItem { label: "Кодировка"; value: "UTF-8" }
            Item { Layout.fillWidth: true }
            Row {
                spacing: 6
                anchors.verticalCenter: parent.verticalCenter
                Rectangle { width: 7; height: 7; radius: 4; color: "#4caf50"; anchors.verticalCenter: parent.verticalCenter }
                Text { text: "Готово"; color: "#a8c0dc"; font.pixelSize: 11; font.family: "Arial" }
            }
        }
    }

    // ── Body ─────────────────────────────────────────────────
    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        // Tab bar
        Rectangle {
            Layout.fillWidth: true
            height: 44
            color: "#1e3a70"

            Rectangle {
                anchors.bottom: parent.bottom
                width: parent.width; height: 1
                color: "#2a4a88"
            }

            TabBar {
                id: tabBar
                anchors.fill: parent
                background: Rectangle { color: "transparent" }
                spacing: 0
                leftPadding: 16

                Repeater {
                    model: [
                        "\U0001F4C5  Задание 1: Даты",
                        "\U0001F393  Задание 2: Деканат",
                        "\u2211  Задание 3: Выражения",
                        "\u2261  Задание 4: Типы"
                    ]
                    delegate: AppTabButton {
                        label: modelData
                        isActive: tabBar.currentIndex === index
                    }
                }
            }
        }

        // Page content
        StackLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            currentIndex: tabBar.currentIndex

            DatesPage     {}
            DeanPage      {}
            ExprPage      {}
            TypeCheckPage {}
        }
    }
}
