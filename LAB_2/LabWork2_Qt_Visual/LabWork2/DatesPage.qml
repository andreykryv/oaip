// DatesPage.qml — Задание 1
// Кнопки уже подключены к dateBE.
// Вам нужно только реализовать методы в Date.cpp и DateController.cpp.
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
    readonly property color cGreen:  "#2e7d32"
    readonly property color cOrange: "#e65100"

    property int selectedRow: -1
    property var resultData: null

    // Дата выбранной строки (для расчёта разницы в реальном времени)
    readonly property string selectedDateVal: {
        if (selectedRow < 0) return ""
        var m = dateList.model
        if (typeof m.get === "function") return m.get(selectedRow).dateVal
        if (m && m[selectedRow])         return m[selectedRow].dateVal
        return ""
    }

    // Вычисляет разницу дней между двумя датами "ДД.ММ.ГГГГ"
    function calcDiff(dateA, dateB) {
        if (!dateA || !dateB || dateA === dateB) return "0"
        // Если C++ подключён — используем его
        if (typeof dateBE !== "undefined") {
            return dateBE.getDuration(dateA, dateB)
        }
        // JS-fallback: парсим ДД.ММ.ГГГГ
        function parse(s) {
            var p = s.split(".")
            if (p.length !== 3) return null
            return new Date(parseInt(p[2]), parseInt(p[1])-1, parseInt(p[0]))
        }
        var a = parse(dateA)
        var b = parse(dateB)
        if (!a || !b || isNaN(a) || isNaN(b)) return "—"
        var diff = Math.round((b - a) / 86400000)
        return (diff >= 0 ? "+" : "") + diff
    }

    // ── Диалог открытия файла ─────────────────────────────────
    AppFileDialog {
        id: openDialog; mode: "open"
        // TODO: когда подключите QtQuick.Dialogs раскомментируйте:
         onAccepted: dateBE.openFile(selectedFile)
    }
    AppFileDialog {
        id: saveDialog; mode: "save"
         onAccepted: dateBE.saveFile(selectedFile)
    }
    AppFileDialog {
        id: appendDialog; mode: "save"
         onAccepted: dateBE.appendToFile(selectedFile, newDateInput.text)
    }

    // ── Диалог ошибок ─────────────────────────────────────────
    AppErrorDialog {
        id: errDialog
    }

    // Слушаем сигнал ошибки от C++
    Connections {
        target: typeof dateBE !== "undefined" ? dateBE : null
        function onError(msg) { errDialog.show(msg) }
        // Обновляем таблицу при изменении модели
        // function onDatesChanged() { /* ListView обновится сам через dateBE.dates */ }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 14
        spacing: 10

        // ── Toolbar ──────────────────────────────────────────
        AppToolBar {
            Layout.fillWidth: true

            AppButton { text: "📂  Открыть файл…"; variant: "primary"
             onClicked: dateBE.openFileDialog() }
            AppButton {  text: "💾  Сохранить";    onClicked: dateBE.saveFileDialog() }

            ToolSep {}
            AppButton {
                text: "+ Добавить дату"
                // ПОДКЛЮЧИТЬ: onClicked: dateBE.addDate(newDateInput.text)
                onClicked: {
                    if (typeof dateBE !== "undefined")
                        dateBE.addDate(newDateInput.text)
                }
            }
            AppButton {
                text: "\u2715 Удалить"; variant: "danger"
                // ПОДКЛЮЧИТЬ: onClicked: dateBE.deleteDate(selectedRow)
                onClicked: {
                    if (typeof dateBE !== "undefined" && selectedRow >= 0) {
                        dateBE.deleteDate(selectedRow)
                        selectedRow = -1
                    }
                }
            }
                        AppButton { text: "Дозапись";     onClicked: dateBE.appendToFileDialog(newDateInput.text) }
            ToolSep {}
            Text { text: "Сегодня:"; color: cDim; font.pixelSize: 12; font.family: "Arial" }
            Text {
                // ПОДКЛЮЧИТЬ: text: typeof dateBE !== "undefined" ? dateBE.todayStr : "..."
                text: typeof dateBE !== "undefined" ? dateBE.todayStr : Qt.formatDate(new Date(),"dd.MM.yyyy")
                color: cAccent; font.pixelSize: 13; font.family: "Arial"; font.weight: Font.Bold
            }
            Item { Layout.fillWidth: true }
            Text { text: "dates.txt"; color: cDim; font.pixelSize: 11; font.family: "Arial" }
        }

        // ── Main area ─────────────────────────────────────────
        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 12

            // ── Left ──────────────────────────────────────────
            ColumnLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                spacing: 10

                Rectangle {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    color: cWhite; border.color: cBorder; border.width: 1; radius: 6; clip: true

                    ColumnLayout {
                        anchors.fill: parent; spacing: 0

                        Rectangle {
                            Layout.fillWidth: true; height: 36; color: cNavy; radius: 6
                            Rectangle { anchors.bottom:parent.bottom; width:parent.width; height:8; color:cNavy }
                            Row {
                                anchors.fill: parent
                                Repeater {
                                    model: [
                                        {lbl:"#",            wd:40 },
                                        {lbl:"Дата",         wd:110},
                                        {lbl:"Следующий день",wd:115},
                                        {lbl:"   Предыд. день", wd:115},
                                        {lbl:"    Разница от
   выбранного",wd:105},
                                        {lbl:"Високосный",   wd:88 },
                                        {lbl:"№ недели",     wd:72 }
                                    ]
                                    delegate: Item {
                                        width:modelData.wd; height:36
                                        Rectangle { anchors.right:parent.right; width:1; height:parent.height; color:"#2a4a88" }
                                        Text { anchors.fill:parent; anchors.leftMargin:8; text:modelData.lbl; color:"#a8c0dc"; font.pixelSize:11; font.family:"Arial"; font.weight:Font.Bold; verticalAlignment:Text.AlignVCenter }
                                    }
                                }
                            }
                        }

                        ListView {
                            id: dateList
                            Layout.fillWidth: true; Layout.fillHeight: true
                            // ПОДКЛЮЧИТЬ модель к C++:
                            // model: typeof dateBE !== "undefined" ? dateBE.dates : demoModel
                            model: typeof dateBE !== "undefined" ? dateBE.dates : demoModel
                            clip: true

                            ScrollBar.vertical: ScrollBar {
                                policy: ScrollBar.AsNeeded
                                contentItem: Rectangle { radius:3; color:"#c8d4e8" }
                                background: Rectangle { color:"transparent" }
                            }

                            delegate: Rectangle {
                                width: dateList.width; height: 36
                                color: selectedRow===index ? "#e0f4f8" : rowHov.hovered ? "#f4f8ff" : (index%2===0?cWhite:"#f8fafd")
                                Behavior on color { ColorAnimation { duration:80 } }
                                HoverHandler { id:rowHov }
                                TapHandler {
                                    onTapped: {
                                        selectedRow = index
                                        // ПОДКЛЮЧИТЬ: вычисляем результат через C++
                                        if (typeof dateBE !== "undefined") {
                                            resultData = dateBE.getResult(
                                                modelData.dateVal,
                                                bdayInput.text
                                            )
                                        }
                                    }
                                }
                                Rectangle { visible:selectedRow===index; width:3; height:parent.height; color:cAccent; radius:2 }
                                Row {
                                    anchors.fill: parent
                                    DateCell { width:40;  cellText:(index+1).toString(); cellColor:cDim }
                                    DateCell { width:110; cellText:modelData.dateVal ? modelData.dateVal : ""; cellColor:cAccent; bold:true }
                                    DateCell { width:115; cellText:modelData.nextDay  ? modelData.nextDay : "—" }
                                    DateCell { width:115; cellText:modelData.prevDay  ? modelData.prevDay : "—" }
                                    Item {
                                        width: 105; height: 36
                                        Rectangle { anchors.right:parent.right; width:1; height:parent.height; color:cBorder }

                                        Text {
                                            anchors.fill: parent
                                            anchors.leftMargin: 10
                                            verticalAlignment: Text.AlignVCenter
                                            font.pixelSize: 13; font.family: "Arial"; font.bold: true
                                            elide: Text.ElideRight

                                            property string diffVal: {
                                                if (selectedRow < 0)   return "—"
                                                if (selectedDateVal === modelData.dateVal) return "● выбрано"
                                                return calcDiff(selectedDateVal, modelData.dateVal)
                                            }

                                            text:  diffVal
                                            color: {
                                                if (diffVal === "—" || diffVal === "● выбрано") return "#7a96b8"
                                                var n = parseInt(diffVal)
                                                if (isNaN(n))  return "#7a96b8"
                                                if (n > 0)     return "#2e7d32"   // зелёный — будущее
                                                if (n < 0)     return "#ef5350"   // красный — прошлое
                                                return "#7a96b8"
                                            }
                                        }
                                    }
                                    Item {
                                        width:88; height:36
                                        Rectangle { anchors.right:parent.right; width:1; height:parent.height; color:cBorder }
                                        Badge { anchors.centerIn:parent; badgeText:modelData.leap?modelData.leap:"—"; badgeColor:modelData.leap==="Да"?"green":"red" }
                                    }
                                    DateCell { width:72; cellText:modelData.weekN?modelData.weekN:"—"; cellColor:cNavy; bold:true }
                                }
                                Rectangle { anchors.bottom:parent.bottom; width:parent.width; height:1; color:cBorder }
                            }

                            // Демо-данные пока C++ не подключён
                            ListModel {
                                id: demoModel
                                ListElement { dateVal:"01.01.0001"; nextDay:"02.01.0001"; prevDay:"31.12.0000"; diff:"\u2014"; leap:"Нет"; weekN:"1"  }
                                ListElement { dateVal:"24.04.2000"; nextDay:"25.04.2000"; prevDay:"23.04.2000"; diff:"730293"; leap:"Да";  weekN:"17" }
                                ListElement { dateVal:"29.02.2000"; nextDay:"01.03.2000"; prevDay:"28.02.2000"; diff:"38";     leap:"Да";  weekN:"9"  }
                                ListElement { dateVal:"31.12.2021"; nextDay:"01.01.2022"; prevDay:"30.12.2021"; diff:"672";    leap:"Нет"; weekN:"52" }
                            }
                        }
                    }
                }

                AppGroupBox {
                    title: "Добавить / изменить дату"
                    Layout.fillWidth: true

                    RowLayout {
                        Layout.fillWidth: true; spacing: 8
                        Text { text:"Дата (ДД.ММ.ГГГГ):"; color:cText; font.pixelSize:13; font.family:"Arial" }
                        AppInput { id:newDateInput; implicitWidth:130; placeholderText:"29.02.2024" }
                        AppButton {
                            text:"Добавить"; variant:"primary"
                            // ПОДКЛЮЧИТЬ: onClicked: dateBE.addDate(newDateInput.text)
                            onClicked: { if (typeof dateBE !== "undefined") dateBE.addDate(newDateInput.text) }
                        }
                        AppButton {
                            text:"Изменить выбранную"
                            // ПОДКЛЮЧИТЬ: onClicked: dateBE.editDate(selectedRow, newDateInput.text)
                            onClicked: { if (typeof dateBE !== "undefined" && selectedRow>=0) dateBE.editDate(selectedRow, newDateInput.text) }
                        }
                        AppButton {
                            text:"Дозапись в файл"
                            onClicked: appendDialog.open()
                        }
                        Item { Layout.fillWidth:true }
                    }
                }
            }

            // ── Right panel ───────────────────────────────────
            ColumnLayout {
                Layout.preferredWidth:255; Layout.minimumWidth:220; Layout.maximumWidth:270
                Layout.fillHeight: true; spacing:10

                AppGroupBox {
                    title:"День рождения"; Layout.fillWidth:true

                    Text { text:"Дата рождения:"; color:cDim; font.pixelSize:11; font.family:"Arial"; font.weight:Font.Bold }
                    AppInput { id:bdayInput; Layout.fillWidth:true; text:"24.04.2000" }
                    AppButton {
                        Layout.fillWidth:true; text:"Дней до ДР  \u2192"; variant:"primary"
                        // ПОДКЛЮЧИТЬ:
                        onClicked: {
                            if (typeof dateBE !== "undefined" && selectedRow >= 0) {
                                var model = dateList.model
                                var dateVal = (typeof model.get === "function")
                                    ? model.get(selectedRow).dateVal
                                    : (dateList.model[selectedRow] ? dateList.model[selectedRow].dateVal : "")
                                resultData = dateBE.getResult(dateVal, bdayInput.text)
                            }
                        }
                    }
                }

                AppGroupBox {
                    title:"Результат"; Layout.fillWidth:true; Layout.fillHeight:true

                    Item {
                        Layout.fillWidth:true; Layout.fillHeight:true
                        visible: selectedRow < 0
                        Text { anchors.centerIn:parent; text:"Выберите строку\nв таблице"; color:cDim; font.pixelSize:12; font.family:"Arial"; horizontalAlignment:Text.AlignHCenter }
                    }

                    ColumnLayout {
                        Layout.fillWidth:true; Layout.fillHeight:true
                        visible: selectedRow >= 0; spacing:4

                        Rectangle {
                            Layout.fillWidth:true; height:34; radius:4; color:cNavy
                            Text {
                                anchors.centerIn:parent
                                text: {
                                    if (selectedRow < 0) return ""
                                    // Берём дату из модели
                                    var m = dateList.model
                                    if (typeof m.get === "function") return m.get(selectedRow).dateVal
                                    if (m[selectedRow]) return m[selectedRow].dateVal
                                    return ""
                                }
                                color:"#ffffff"; font.pixelSize:14; font.family:"Arial"; font.weight:Font.Bold
                            }
                        }

                        Repeater {
                            model: resultData ? [
                                {k:"Следующий день",  v: resultData.next     || "—", hi:false},
                                {k:"Предыдущий день", v: resultData.prev     || "—", hi:false},
                                {k:"Високосный год",  v: resultData.leap     || "—", leap:true},
                                {k:"Номер недели",    v: resultData.week     || "—", hi:false},
                                {k:"Дней до ДР",      v: resultData.tillBday || "—", hi:false}
                            ] : []
                            delegate: Rectangle {
                                Layout.fillWidth:true; height:30
                                color:index%2===0?"#f8fafd":cWhite; radius:3
                                RowLayout {
                                    anchors.fill:parent; anchors.leftMargin:8; anchors.rightMargin:8
                                    Text { Layout.fillWidth:true; text:modelData.k; color:cDim; font.pixelSize:11; font.family:"Arial" }
                                    Text {
                                        text:modelData.v
                                        color: modelData.leap ? (modelData.v==="Да"?cGreen:cRed) : cNavy
                                        font.pixelSize:13; font.family:"Arial"; font.weight:Font.Bold
                                    }
                                }
                            }
                        }
                        Item { Layout.fillHeight:true }
                    }
                }
            }
        }
    }
}
