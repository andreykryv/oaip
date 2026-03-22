// DeanPage.qml
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {
    readonly property color cNavy:   "#1a2f5e"
    readonly property color cAccent: "#0097b2"
    readonly property color cRed:    "#ef5350"
    readonly property color cWhite:  "#ffffff"
    readonly property color cBorder: "#dce6f0"
    readonly property color cText:   "#1a2f5e"
    readonly property color cDim:    "#7a96b8"
    readonly property color cBg:     "#f8fafd"
    readonly property color cGreen:  "#2e7d32"
    readonly property color cOrange: "#e65100"

    function gradeAvg(g1,g2,g3,g4,g5) { return ((g1+g2+g3+g4+g5)/5).toFixed(2) }

    ListModel {
        id: studentModel
        ListElement { grp:"ИВТ-21"; nm:"Иванов А.А.";   w1:5;w2:4;w3:3;w4:5;w5:4; s1:5;s2:4;s3:4;s4:5;s5:5; st:"ok"    }
        ListElement { grp:"ИВТ-21"; nm:"Петрова М.С.";  w1:3;w2:2;w3:4;w4:3;w5:4; s1:4;s2:3;s3:3;s4:4;s5:4; st:"debt"  }
        ListElement { grp:"ИВТ-22"; nm:"Сидоров Д.О.";  w1:2;w2:3;w3:2;w4:4;w5:3; s1:2;s2:3;s3:2;s4:4;s5:3; st:"expel" }
        ListElement { grp:"ИВТ-22"; nm:"Козлова Е.В.";  w1:5;w2:5;w3:4;w4:5;w5:5; s1:5;s2:5;s3:5;s4:5;s5:4; st:"ok"    }
        ListElement { grp:"ИВТ-23"; nm:"Морозов К.П.";  w1:4;w2:4;w3:3;w4:4;w5:3; s1:2;s2:2;s3:3;s4:4;s5:3; st:"expel" }
        ListElement { grp:"ИВТ-23"; nm:"Волкова Н.Н.";  w1:3;w2:2;w3:4;w4:4;w5:3; s1:3;s2:3;s3:2;s4:2;s5:3; st:"debt"  }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 14
        anchors.leftMargin: 14
        anchors.rightMargin: 14
        anchors.topMargin: 14
        anchors.bottomMargin: 14
        spacing: 10

        AppToolBar {
            Layout.fillWidth: true
            AppButton { text: "\U0001F4C2  Открыть файл\u2026";
                variant: "primary";
                onClicked: studentBE.openFile()   }
            AppButton { text: "\U0001F4BE  Сохранить";
                onClicked:  studentBE.saveFile()  }
            ToolSep {}
            AppButton { text: "+ Добавить студента" }
            AppButton { text: "\u2715 Удалить"; variant: "danger" }
            ToolSep {}
            Text { text: "Фильтр:"; color: cDim; font.pixelSize: 12; font.family: "Arial" }
            AppComboBox {
                id: filterCombo; implicitWidth: 235
                onCurrentIndexChanged:  studentListView.model = studentBE.filtered(currentIndex, searchInput.text)
                model: ["Все студенты", "Задолжники (зимняя сессия)", "К отчислению (\u22652 неуд + долг)"]
            }
            ToolSep {}
            Text { text: "Поиск:"; color: cDim; font.pixelSize: 12; font.family: "Arial" }
            AppInput { id: searchInput; implicitWidth: 150; placeholderText: "\u0424\u0418\u041e\u2026" }
        }

        // Table
        Rectangle {
            Layout.fillWidth: true; Layout.fillHeight: true
            color: cWhite; border.color: cBorder; border.width: 1; radius: 6

            ColumnLayout {
                anchors.fill: parent; spacing: 0

                // Header
                Rectangle {
                    Layout.fillWidth: true; height: 36
                    color: cNavy; radius: 6
                    Rectangle { anchors.bottom: parent.bottom; width: parent.width; height: 8; color: cNavy }

                    Row {
                        anchors.fill: parent
                        Repeater {
                            model: [
                                {lbl:"Группа",       wd:82 },
                                {lbl:"ФИО",          wd:158},
                                {lbl:"Зима: оценки", wd:155},
                                {lbl:"Ср. зима",     wd:78 },
                                {lbl:"Лето: оценки", wd:155},
                                {lbl:"Ср. лето",     wd:78 },
                                {lbl:"Статус",       wd:108},
                                {lbl:"Действия",     wd:82 }
                            ]
                            delegate: Item {
                                width: modelData.wd; height: 36
                                Rectangle { anchors.right:parent.right; width:1; height:parent.height; color:"#2a4a88" }
                                Text {
                                    anchors.fill: parent; anchors.leftMargin: 10
                                    text: modelData.lbl; color: "#a8c0dc"
                                    font.pixelSize: 11; font.family: "Arial"; font.weight: Font.Bold
                                    verticalAlignment: Text.AlignVCenter
                                }
                            }
                        }
                    }
                }

                ListView {
                    id: studentList; Layout.fillWidth: true; Layout.fillHeight: true
                    model: typeof studentBE !== "undefined" ? studentBE.filtered(filterCombo.currentIndex, searchInput.text) : studentModel; clip: true

                    ScrollBar.vertical: ScrollBar {
                        policy: ScrollBar.AsNeeded
                        contentItem: Rectangle { radius: 3; color: "#c8d4e8" }
                        background: Rectangle { color: "transparent" }
                    }

                    delegate: Rectangle {
                        width: studentList.width; height: 38
                        property var d: (modelData !== undefined && typeof modelData === "object")
                    ? modelData : model
                        color: {
                            if (d.st === "expel") return "#fff5f5"
                            if (d.st === "debt")  return "#fffde7"
                            return sHov.hovered ? "#f4f8ff" : (index%2===0 ? cWhite : "#f8fafd")
                        }
                        HoverHandler { id: sHov }

                        // Status stripe on left
                        Rectangle {
                            width: 3; height: parent.height; radius: 2
                            color: d.st === "expel" ? cRed : d.st === "debt" ? "#ffa726" : "#4caf50"
                        }

                        Row {
                            anchors.fill: parent
                            DCell { width:82;  cellText:modelData.grp; cellColor:cNavy; bold:true }
                            DCell { width:158; cellText:modelData.nm;  cellColor:cText }
                            Item {
                                width:155; height:38
                                Rectangle { anchors.right:parent.right; width:1; height:parent.height; color:cBorder }
                                Row {
                                    anchors.verticalCenter:parent.verticalCenter
                                    anchors.left:parent.left; anchors.leftMargin:8
                                    spacing: 4
                                    Repeater {
                                        model:[d.w1,d.w2,d.w3,d.w4,d.w5]
                                        delegate: Badge {
                                            badgeText: modelData.toString()
                                            badgeColor: modelData<3?"red":modelData===5?"green":"blue"
                                        }
                                    }
                                }
                            }
                            DCell {
                                width:78; bold:true
                                cellText: gradeAvg(d.w1,d.w2,d.w3,d.w4,d.w5)
                                cellColor: parseFloat(gradeAvg(d.w1,d.w2,d.w3,d.w4,d.w5))<3 ? cRed :
                                           parseFloat(gradeAvg(d.w1,d.w2,d.w3,d.w4,d.w5))>=4.5 ? cGreen : cOrange
                            }
                            Item {
                                width:155; height:38
                                Rectangle { anchors.right:parent.right; width:1; height:parent.height; color:cBorder }
                                Row {
                                    anchors.verticalCenter:parent.verticalCenter
                                    anchors.left:parent.left; anchors.leftMargin:8
                                    spacing: 4
                                    Repeater {
                                        model:[d.s1,d.s2,d.s3,d.s4,d.s5]
                                        delegate: Badge {
                                            badgeText: modelData.toString()
                                            badgeColor: modelData<3?"red":modelData===5?"green":"blue"
                                        }
                                    }
                                }
                            }
                            DCell {
                                width:78; bold:true
                                cellText: gradeAvg(d.s1,d.s2,d.s3,d.s4,d.s5)
                                cellColor: parseFloat(gradeAvg(d.s1,d.s2,d.s3,d.s4,d.s5))<3 ? cRed :
                                           parseFloat(gradeAvg(d.s1,d.s2,d.s3,d.s4,d.s5))>=4.5 ? cGreen : cOrange
                            }
                            Item {
                                width:108; height:38
                                Rectangle { anchors.right:parent.right; width:1; height:parent.height; color:cBorder }
                                Badge {
                                    anchors.centerIn: parent
                                    badgeText:  d.st==="expel"?"ОТЧИСЛИТЬ":d.st==="debt"?"ДОЛГ":"OK"
                                    badgeColor: d.st==="expel"?"red":d.st==="debt"?"yellow":"green"
                                }
                            }
                            Item {
                                width:82; height:38
                                Row {
                                    anchors.centerIn:parent; spacing:4
                                    AppButton { text:"\u270F"; implicitWidth:30; leftPadding:4; rightPadding:4 }
                                    AppButton { text:"\u2715"; variant:"danger"; implicitWidth:30; leftPadding:4; rightPadding:4 }
                                }
                            }
                        }
                        Rectangle { anchors.bottom:parent.bottom; width:parent.width; height:1; color:cBorder }
                    }
                }
            }
        }

        // Sort bar
        AppToolBar {
            Layout.fillWidth: true
            Text { text: "Сортировка:"; color: cDim; font.pixelSize: 12; font.family: "Arial" }
            AppButton { text: "По ФИО \u2191";
                onClicked:  if(typeof studentBE!=="undefined") studentBE.sortByName() }
            AppButton { text: "По группе";
                onClicked:  if(typeof studentBE!=="undefined") studentBE.sortByGroup()  }
            AppButton { text: "По ср. баллу \u2193";
                onClicked:  if(typeof studentBE!=="undefined") studentBE.sortByAvgDesc()  }
            ToolSep {}
            Rectangle { width:10;height:10;radius:5; color:cRed;    Layout.alignment:Qt.AlignVCenter }
            Text { text:"Отчислить:"; color:cDim; font.pixelSize:12; font.family:"Arial" }
            Text { text: typeof studentBE!=="undefined" ? studentBE.expelCount.toString() : "2"; color:cRed;    font.pixelSize:13; font.family:"Arial"; font.weight:Font.Bold }
            Rectangle { width:10;height:10;radius:5; color:"#ffa726"; Layout.alignment:Qt.AlignVCenter }
            Text { text:"Должники:";  color:cDim; font.pixelSize:12; font.family:"Arial" }
            Text { text: typeof studentBE!=="undefined" ? studentBE.debtCount.toString()  : "2"; color:"#e65100"; font.pixelSize:13; font.family:"Arial"; font.weight:Font.Bold }
            Rectangle { width:10;height:10;radius:5; color:"#4caf50"; Layout.alignment:Qt.AlignVCenter }
            Text { text:"OK:"; color:cDim; font.pixelSize:12; font.family:"Arial" }
            Text { text: typeof studentBE!=="undefined" ? studentBE.okCount.toString()    : "2"; color:cGreen;  font.pixelSize:13; font.family:"Arial"; font.weight:Font.Bold }
            Item { Layout.fillWidth:true }
        }
    }
}
