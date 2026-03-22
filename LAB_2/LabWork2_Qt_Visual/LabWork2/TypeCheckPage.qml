// TypeCheckPage.qml
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {
    readonly property color cNavy:   "#1a2f5e"
    readonly property color cAccent: "#0097b2"
    readonly property color cRed:    "#ef5350"
    readonly property color cWhite:  "#ffffff"
    readonly property color cBorder: "#dce6f0"
    readonly property color cDim:    "#7a96b8"
    readonly property color cGreen:  "#2e7d32"
    readonly property color cBg:     "#f0f4fa"

    readonly property var typeNames: ["Number", "BinaryOperation"]
    readonly property var typeTags:  [1, 2]

    property bool checkResult: leftCombo.currentIndex === rightCombo.currentIndex

    readonly property string codeHtml:
        "<span style='color:#629755;font-style:italic'>// Добавляем виртуальный тег в базовый класс</span><br>" +
        "<span style='color:#7952b3'>struct</span> <span style='color:#1a2f5e'>Expression</span> {<br>" +
        "&nbsp;&nbsp;<span style='color:#7952b3'>virtual</span> <span style='color:#7952b3'>double</span> " +
        "<span style='color:#0097b2'>evaluate</span>() = <span style='color:#2e7d32'>0</span>;<br>" +
        "&nbsp;&nbsp;<span style='color:#7952b3'>virtual</span> <span style='color:#7952b3'>int</span> " +
        "<span style='color:#0097b2'>type_tag</span>() <span style='color:#7952b3'>const</span> = <span style='color:#2e7d32'>0</span>;<br>" +
        "&nbsp;&nbsp;<span style='color:#7952b3'>virtual</span> ~<span style='color:#0097b2'>Expression</span>()" +
        " = <span style='color:#7952b3'>default</span>;<br>};<br><br>" +
        "<span style='color:#629755;font-style:italic'>// Number \u2192 tag 1</span><br>" +
        "<span style='color:#7952b3'>struct</span> <span style='color:#1a2f5e'>Number</span> : <span style='color:#1a2f5e'>Expression</span> {<br>" +
        "&nbsp;&nbsp;<span style='color:#7952b3'>int</span> <span style='color:#0097b2'>type_tag</span>()" +
        " <span style='color:#7952b3'>const override</span> { <span style='color:#7952b3'>return</span> <span style='color:#2e7d32'>1</span>; }<br>};<br><br>" +
        "<span style='color:#629755;font-style:italic'>// BinaryOperation \u2192 tag 2</span><br>" +
        "<span style='color:#7952b3'>struct</span> <span style='color:#1a2f5e'>BinaryOperation</span> : <span style='color:#1a2f5e'>Expression</span> {<br>" +
        "&nbsp;&nbsp;<span style='color:#7952b3'>int</span> <span style='color:#0097b2'>type_tag</span>()" +
        " <span style='color:#7952b3'>const override</span> { <span style='color:#7952b3'>return</span> <span style='color:#2e7d32'>2</span>; }<br>};<br><br>" +
        "<span style='color:#629755;font-style:italic'>// Без typeid / dynamic_cast</span><br>" +
        "<span style='color:#7952b3'>bool</span> <span style='color:#0097b2'>check_equals</span>(<br>" +
        "&nbsp;&nbsp;<span style='color:#1a2f5e'>Expression</span> <span style='color:#7952b3'>const</span>* left,<br>" +
        "&nbsp;&nbsp;<span style='color:#1a2f5e'>Expression</span> <span style='color:#7952b3'>const</span>* right)<br>{<br>" +
        "&nbsp;&nbsp;<span style='color:#7952b3'>return</span> left-><span style='color:#0097b2'>type_tag</span>()" +
        " == right-><span style='color:#0097b2'>type_tag</span>();<br>}"

    RowLayout {
        anchors.fill: parent
        anchors.margins: 14
        spacing: 12

        // ── Left: code panel ──────────────────────────────────
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: cWhite; border.color: cBorder; border.width: 1; radius: 6
            clip: true

            ColumnLayout {
                anchors.fill: parent
                spacing: 0

                // File tabs
                Rectangle {
                    Layout.fillWidth: true; height: 38
                    color: cBg; radius: 6
                    Rectangle { anchors.bottom:parent.bottom; width:parent.width; height:8; color:cBg }
                    Rectangle { anchors.bottom:parent.bottom; width:parent.width; height:1; color:cBorder }
                    Row {
                        anchors.fill: parent; anchors.leftMargin: 8; spacing: 0
                        Repeater {
                            model: ["expression.h", "check_equals.cpp"]
                            delegate: Rectangle {
                                width: ctl.implicitWidth + 24; height: 38
                                color:        index === 1 ? cWhite  : "transparent"
                                border.color: index === 1 ? cBorder : "transparent"
                                border.width: 1
                                Rectangle {
                                    anchors.bottom: parent.bottom
                                    width: parent.width; height: index === 1 ? 2 : 0
                                    color: cAccent
                                }
                                Text {
                                    id: ctl; anchors.centerIn: parent; text: modelData
                                    color: index === 1 ? cNavy : cDim
                                    font.pixelSize: 12; font.family: "Arial"
                                    font.weight: index === 1 ? Font.Bold : Font.Normal
                                }
                            }
                        }
                    }
                }

                // Code
                Flickable {
                    Layout.fillWidth: true; Layout.fillHeight: true
                    contentHeight: tcEdit.implicitHeight + 24; clip: true

                    ScrollBar.vertical: ScrollBar {
                        policy: ScrollBar.AsNeeded
                        contentItem: Rectangle { radius: 3; color: "#c8d4e8" }
                        background: Rectangle { color: "transparent" }
                    }

                    Rectangle {
                        x:0; y:0; width:36
                        height: Math.max(parent.contentHeight, parent.height)
                        color: cBg
                        Rectangle { anchors.right:parent.right; width:1; height:parent.height; color:cBorder }
                        Column {
                            anchors.fill:parent; anchors.topMargin:14
                            Repeater {
                                model: 30
                                delegate: Text {
                                    width:30; text:index+1; color:cDim
                                    font.pixelSize:11; font.family:"Consolas"
                                    horizontalAlignment:Text.AlignRight; height:20
                                }
                            }
                        }
                    }

                    TextEdit {
                        id: tcEdit; x:44; y:14
                        width: parent.width - 50
                        readOnly:true; selectByMouse:true
                        font.pixelSize:13; font.family:"Consolas"
                        color:cNavy; wrapMode:TextEdit.NoWrap
                        textFormat:TextEdit.RichText; text:codeHtml
                    }
                }

                // OCP note
                Rectangle {
                    Layout.fillWidth: true; height: 46
                    color: cBg
                    Rectangle { anchors.top:parent.top; width:parent.width; height:1; color:cBorder }
                    RowLayout {
                        anchors.fill:parent; anchors.margins:12; spacing:10
                        Rectangle {
                            width:42; height:22; radius:11
                            color:"#e0f4f8"; border.color:"#b2e0ea"; border.width:1
                            Text { anchors.centerIn:parent; text:"OCP"; color:cNavy; font.pixelSize:10; font.family:"Arial"; font.weight:Font.Bold }
                        }
                        Text {
                            Layout.fillWidth:true
                            text: "Новый класс добавляет type_tag() с уникальным тегом.\nБазовый класс не изменяется — Open/Closed Principle."
                            color:cDim; font.pixelSize:11; font.family:"Arial"; wrapMode:Text.Wrap
                        }
                    }
                }
            }
        }

        // ── Right: demo panel ─────────────────────────────────
        // Fixed width — does NOT fill, so it won't overlap the code panel
        ColumnLayout {
            Layout.preferredWidth: 285
            Layout.minimumWidth:   260
            Layout.maximumWidth:   300
            Layout.fillHeight: true
            spacing: 10

            // Demo
            AppGroupBox {
                title: "Демо: check_equals(left, right)"
                Layout.fillWidth: true

                RowLayout {
                    Layout.fillWidth: true
                    Text { text:"left  ="; color:cDim; font.pixelSize:13; font.family:"Arial"; font.weight:Font.Bold; width:60 }
                    AppComboBox { id:leftCombo; Layout.fillWidth:true; model:typeNames }
                }
                RowLayout {
                    Layout.fillWidth: true
                    Text { text:"right ="; color:cDim; font.pixelSize:13; font.family:"Arial"; font.weight:Font.Bold; width:60 }
                    AppComboBox { id:rightCombo; Layout.fillWidth:true; model:typeNames }
                }

                Rectangle {
                    Layout.fillWidth: true; height: 88; radius: 8
                    color:        checkResult ? "#e8f5e9" : "#fdecea"
                    border.color: checkResult ? "#a5d6a7" : "#ef9a9a"
                    border.width: 1
                    Behavior on color { ColorAnimation { duration: 150 } }

                    Column {
                        anchors.centerIn: parent; spacing: 4
                        Text {
                            anchors.horizontalCenter: parent.horizontalCenter
                            text: "check_equals(left, right)"
                            color: cDim; font.pixelSize: 11; font.family: "Arial"
                        }
                        Text {
                            anchors.horizontalCenter: parent.horizontalCenter
                            text: checkResult ? "true" : "false"
                            color: checkResult ? cGreen : cRed
                            font.pixelSize: 30; font.family: "Arial"; font.weight: Font.Bold
                            Behavior on color { ColorAnimation { duration: 150 } }
                        }
                        Text {
                            anchors.horizontalCenter: parent.horizontalCenter
                            text: {
                                var li = leftCombo.currentIndex
                                var ri = rightCombo.currentIndex
                                if (li === ri) return "оба: tag = " + typeTags[li]
                                return typeNames[li]+": tag="+typeTags[li]+" \u2260 "+typeNames[ri]+": tag="+typeTags[ri]
                            }
                            color: cDim; font.pixelSize: 11; font.family: "Arial"
                        }
                    }
                }

                AppButton {
                    Layout.fillWidth:true; text:"\u25B6  Вызвать check_equals()"; variant:"primary"
                    onClicked: {
                        if (typeof exprBE !== "undefined") {
                            // ── Вызов C++ check_equals ─────────────
                            var result = exprBE.checkEquals(
                                leftCombo.currentIndex,
                                rightCombo.currentIndex
                            )
                            // result автоматически обновит checkResult
                            // через привязку leftCombo.currentIndex === rightCombo.currentIndex
                        }
                    }
                }
            }

            // Tags
            AppGroupBox {
                title: "Теги классов"
                Layout.fillWidth: true

                Repeater {
                    model: [{nm:"Number",tag:1},{nm:"BinaryOperation",tag:2}]
                    delegate: Rectangle {
                        Layout.fillWidth: true; height: 34
                        color: index % 2 === 0 ? cWhite : "#f8fafd"
                        Rectangle { anchors.bottom:parent.bottom; width:parent.width; height:1; color:cBorder }
                        RowLayout {
                            anchors.fill:parent; anchors.leftMargin:8; anchors.rightMargin:8
                            Text { text:modelData.nm; color:cNavy; font.pixelSize:13; font.family:"Arial"; Layout.fillWidth:true }
                            Badge { badgeText:"tag = "+modelData.tag; badgeColor:"blue" }
                        }
                    }
                }
            }

            // Forbidden
            AppGroupBox {
                title: "Запрещено"
                Layout.fillWidth: true

                Repeater {
                    model: ["typeid()", "dynamic_cast<>()", "Вызов методов по указателю"]
                    delegate: RowLayout {
                        Layout.fillWidth: true
                        Badge { badgeText:"\u2715"; badgeColor:"red" }
                        Text { text:modelData; color:cRed; font.pixelSize:12; font.family:"Arial"; Layout.fillWidth:true }
                    }
                }
            }

            Item { Layout.fillHeight: true }
        }
    }
}
