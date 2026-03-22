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

    // ── Выбранная строка (скопировано с DatesPage) ────────────
    property int selectedRow: -1

    // ─────────────────────────────────────────────────────────
    //  Popup: форма добавления / редактирования
    //  editIndex == -1  → добавление
    //  editIndex >= 0   → редактирование
    // ─────────────────────────────────────────────────────────
    Popup {
        id: studentPopup
        property int editIndex: -1

        x: (parent ? (parent.width  - width)  / 2 : 0)
        y: (parent ? (parent.height - height) / 2 : 0)
        modal: true; focus: true; width: 400; padding: 0
        closePolicy: Popup.CloseOnEscape

        function openAdd() {
            editIndex     = -1
            grpInput.text = ""
            nmInput.text  = ""
            for (var i = 0; i < 5; ++i) {
                winterInputs.itemAt(i).text = ""
                summerInputs.itemAt(i).text = ""
            }
            open()
        }

        function openEdit(idx) {
            if (typeof studentBE === "undefined") return
            var s = studentBE.getStudent(idx)
            if (!s) return
            editIndex     = idx
            grpInput.text = s.grp
            nmInput.text  = s.nm
            winterInputs.itemAt(0).text = s.w1
            winterInputs.itemAt(1).text = s.w2
            winterInputs.itemAt(2).text = s.w3
            winterInputs.itemAt(3).text = s.w4
            winterInputs.itemAt(4).text = s.w5
            summerInputs.itemAt(0).text = s.s1
            summerInputs.itemAt(1).text = s.s2
            summerInputs.itemAt(2).text = s.s3
            summerInputs.itemAt(3).text = s.s4
            summerInputs.itemAt(4).text = s.s5
            open()
        }

        background: Rectangle {
            color: "#ffffff"; border.color: cBorder; border.width: 1; radius: 6
        }

        ColumnLayout {
            width: parent.width; spacing: 0

            // Заголовок попапа
            Rectangle {
                Layout.fillWidth: true; height: 42; color: cNavy; radius: 6
                Rectangle { anchors.bottom:parent.bottom; width:parent.width; height:8; color:cNavy }
                Text {
                    anchors.fill: parent; anchors.leftMargin: 16
                    text: studentPopup.editIndex < 0 ? "Добавить студента"
                                                     : "Редактировать студента"
                    color: "#ffffff"; font.pixelSize: 14
                    font.family: "Arial"; font.weight: Font.Bold
                    verticalAlignment: Text.AlignVCenter
                }
            }

            ColumnLayout {
                Layout.fillWidth: true; Layout.margins: 16; spacing: 10

                // Группа
                RowLayout {
                    Layout.fillWidth: true
                    Text { text:"Группа:"; color:cDim; font.pixelSize:13; font.family:"Arial"; width:80 }
                    AppInput { id:grpInput; Layout.fillWidth:true; placeholderText:"ИВТ-21" }
                }
                // ФИО
                RowLayout {
                    Layout.fillWidth: true
                    Text { text:"ФИО:"; color:cDim; font.pixelSize:13; font.family:"Arial"; width:80 }
                    AppInput { id:nmInput; Layout.fillWidth:true; placeholderText:"Иванов А.А." }
                }

                // Зимние оценки
                ColumnLayout {
                    Layout.fillWidth: true; spacing: 4
                    Text { text:"Зимняя сессия (5 оценок):"; color:cNavy; font.pixelSize:12; font.family:"Arial"; font.weight:Font.Bold }
                    RowLayout {
                        Layout.fillWidth: true; spacing: 6
                        Repeater {
                            id: winterInputs; model: 5
                            delegate: AppInput {
                                Layout.fillWidth: true
                                placeholderText: (index+1).toString()
                                inputMethodHints: Qt.ImhDigitsOnly
                            }
                        }
                    }
                }

                // Летние оценки
                ColumnLayout {
                    Layout.fillWidth: true; spacing: 4
                    Text { text:"Летняя сессия (5 оценок):"; color:cNavy; font.pixelSize:12; font.family:"Arial"; font.weight:Font.Bold }
                    RowLayout {
                        Layout.fillWidth: true; spacing: 6
                        Repeater {
                            id: summerInputs; model: 5
                            delegate: AppInput {
                                Layout.fillWidth: true
                                placeholderText: (index+1).toString()
                                inputMethodHints: Qt.ImhDigitsOnly
                            }
                        }
                    }
                }

                // Кнопки
                RowLayout {
                    Layout.fillWidth: true; spacing: 8
                    Item { Layout.fillWidth: true }
                    AppButton {
                        text: "Отмена"
                        onClicked: studentPopup.close()
                    }
                    AppButton {
                        text: studentPopup.editIndex < 0 ? "Добавить" : "Сохранить"
                        variant: "primary"
                        onClicked: {
                            if (typeof studentBE === "undefined") {
                                studentPopup.close(); return
                            }
                            var w = [], s = []
                            for (var i = 0; i < 5; ++i) {
                                w.push(parseInt(winterInputs.itemAt(i).text) || 0)
                                s.push(parseInt(summerInputs.itemAt(i).text) || 0)
                            }
                            if (studentPopup.editIndex < 0)
                                studentBE.addStudent(grpInput.text, nmInput.text, w, s)
                            else
                                studentBE.editStudent(studentPopup.editIndex,
                                                      grpInput.text, nmInput.text, w, s)
                            studentPopup.close()
                        }
                    }
                }
            }
        }
    }

    // ── Диалог ошибок ─────────────────────────────────────────
    AppErrorDialog { id: errDialog }

    // ── Подписка на сигналы C++ (скопировано с DatesPage) ────
    Connections {
        target: typeof studentBE !== "undefined" ? studentBE : null
        function onError(msg) { errDialog.show(msg) }
        function onStudentsChanged() {
            studentList.model = studentBE.filtered(
                filterCombo.currentIndex, searchInput.text)
        }
    }

    // ── Демо-данные пока C++ не подключён ─────────────────────
    ListModel {
        id: studentModel
        ListElement { grp:"ИВТ-21"; nm:"Иванов А.А.";  w1:5;w2:4;w3:3;w4:5;w5:4; s1:5;s2:4;s3:4;s4:5;s5:5; avgW:"4.20"; avgS:"4.60"; st:"ok";    idx:0 }
        ListElement { grp:"ИВТ-21"; nm:"Петрова М.С."; w1:3;w2:2;w3:4;w4:3;w5:4; s1:4;s2:3;s3:3;s4:4;s5:4; avgW:"3.20"; avgS:"3.60"; st:"debt";  idx:1 }
        ListElement { grp:"ИВТ-22"; nm:"Сидоров Д.О."; w1:2;w2:3;w3:2;w4:4;w5:3; s1:2;s2:3;s3:2;s4:4;s5:3; avgW:"2.80"; avgS:"2.80"; st:"expel"; idx:2 }
        ListElement { grp:"ИВТ-22"; nm:"Козлова Е.В."; w1:5;w2:5;w3:4;w4:5;w5:5; s1:5;s2:5;s3:5;s4:5;s5:4; avgW:"4.80"; avgS:"4.80"; st:"ok";    idx:3 }
        ListElement { grp:"ИВТ-23"; nm:"Морозов К.П."; w1:4;w2:4;w3:3;w4:4;w5:3; s1:2;s2:2;s3:3;s4:4;s5:3; avgW:"3.60"; avgS:"2.80"; st:"expel"; idx:4 }
        ListElement { grp:"ИВТ-23"; nm:"Волкова Н.Н."; w1:3;w2:2;w3:4;w4:4;w5:3; s1:3;s2:3;s3:2;s4:2;s5:3; avgW:"3.20"; avgS:"2.60"; st:"debt";  idx:5 }
    }

    ColumnLayout {
        anchors.fill: parent; anchors.margins: 14; spacing: 10

        // ── Тулбар ───────────────────────────────────────────
        AppToolBar {
            Layout.fillWidth: true

            AppButton {
                text: "📂  Открыть файл…"; variant: "primary"
                // Вызывает QFileDialog::getOpenFileName напрямую из C++.
                // Идентично кнопке на вкладке Даты.
                onClicked: { if (typeof studentBE !== "undefined") studentBE.openFileDialog() }
            }
            AppButton {
                text: "💾  Сохранить"
                // Вызывает QFileDialog::getSaveFileName напрямую из C++.
                onClicked: { if (typeof studentBE !== "undefined") studentBE.saveFileDialog() }
            }
            ToolSep {}
            AppButton {
                text: "+ Добавить студента"
                onClicked: studentPopup.openAdd()
            }
            AppButton {
                text: "✕ Удалить"; variant: "danger"
                onClicked: {
                    if (typeof studentBE !== "undefined" && selectedRow >= 0) {
                        studentBE.deleteStudent(selectedRow)
                        selectedRow = -1
                    }
                }
            }
            ToolSep {}
            Text { text:"Фильтр:"; color:cDim; font.pixelSize:12; font.family:"Arial" }
            AppComboBox {
                id: filterCombo; implicitWidth: 235
                onCurrentIndexChanged: {
                    if (typeof studentBE !== "undefined")
                        studentList.model = studentBE.filtered(currentIndex, searchInput.text)
                }
                model: ["Все студенты","Задолжники (зимняя сессия)","К отчислению (≥2 неуд + долг)"]
            }
            ToolSep {}
            Text { text:"Поиск:"; color:cDim; font.pixelSize:12; font.family:"Arial" }
            AppInput {
                id: searchInput; implicitWidth: 150; placeholderText: "ФИО…"
                onTextChanged: {
                    if (typeof studentBE !== "undefined")
                        studentList.model = studentBE.filtered(filterCombo.currentIndex, text)
                }
            }
        }

        // ── Таблица ───────────────────────────────────────────
        Rectangle {
            Layout.fillWidth: true; Layout.fillHeight: true
            color: cWhite; border.color: cBorder; border.width: 1; radius: 6

            ColumnLayout {
                anchors.fill: parent; spacing: 0

                // Заголовок с кликабельными колонками сортировки
                Rectangle {
                    Layout.fillWidth: true; height: 36; color: cNavy; radius: 6
                    Rectangle { anchors.bottom:parent.bottom; width:parent.width; height:8; color:cNavy }

                    Row {
                        anchors.fill: parent
                        Repeater {
                            model: [
                                {lbl:"#",            wd:40,  col:""},
                                {lbl:"Группа",       wd:82,  col:"group"},
                                {lbl:"ФИО",          wd:158, col:"name"},
                                {lbl:"Зима: оценки", wd:155, col:""},
                                {lbl:"Ср. зима",     wd:78,  col:""},
                                {lbl:"Лето: оценки", wd:155, col:""},
                                {lbl:"Ср. лето",     wd:78,  col:"avg"},
                                {lbl:"Статус",       wd:108, col:""},
                                {lbl:"Действия",     wd:82,  col:""}
                            ]
                            delegate: Item {
                                width: modelData.wd; height: 36
                                Rectangle { anchors.right:parent.right; width:1; height:parent.height; color:"#2a4a88" }

                                MouseArea {
                                    anchors.fill: parent
                                    enabled: modelData.col !== ""
                                    cursorShape: modelData.col !== "" ? Qt.PointingHandCursor : Qt.ArrowCursor
                                    onClicked: {
                                        if (typeof studentBE === "undefined") return
                                        if (modelData.col === "name")  studentBE.sortByName()
                                        if (modelData.col === "group") studentBE.sortByGroup()
                                        if (modelData.col === "avg")   studentBE.sortByAvgDesc()
                                    }
                                }

                                RowLayout {
                                    anchors.fill: parent; anchors.leftMargin: 8; spacing: 2
                                    Text {
                                        text: modelData.lbl
                                        color: (typeof studentBE !== "undefined"
                                                && studentBE.sortColumn === modelData.col
                                                && modelData.col !== "") ? "#ffffff" : "#a8c0dc"
                                        font.pixelSize: 11; font.family: "Arial"; font.weight: Font.Bold
                                        verticalAlignment: Text.AlignVCenter
                                    }
                                    Text {
                                        visible: typeof studentBE !== "undefined"
                                                 && studentBE.sortColumn === modelData.col
                                                 && modelData.col !== ""
                                        text: (typeof studentBE !== "undefined" && studentBE.sortAsc) ? "↑" : "↓"
                                        color: "#00bcd4"; font.pixelSize: 12; font.weight: Font.Bold
                                    }
                                }
                            }
                        }
                    }
                }

                // ── Список студентов ──────────────────────────
                ListView {
                    id: studentList
                    Layout.fillWidth: true; Layout.fillHeight: true
                    model: typeof studentBE !== "undefined"
                           ? studentBE.filtered(filterCombo.currentIndex, searchInput.text)
                           : studentModel
                    clip: true

                    ScrollBar.vertical: ScrollBar {
                        policy: ScrollBar.AsNeeded
                        contentItem: Rectangle { radius:3; color:"#c8d4e8" }
                        background: Rectangle { color:"transparent" }
                    }

                    delegate: Rectangle {
                        width: studentList.width; height: 38
                        color: {
                            if (selectedRow === index)        return "#e0f4f8"
                            if (modelData.st === "expel")     return "#fff5f5"
                            if (modelData.st === "debt")      return "#fffde7"
                            return rowHov.hovered ? "#f4f8ff" : (index%2===0 ? cWhite : "#f8fafd")
                        }
                        Behavior on color { ColorAnimation { duration: 80 } }
                        HoverHandler { id: rowHov }

                        // Скопировано с DatesPage: выбор строки кликом
                        TapHandler { onTapped: selectedRow = index }

                        // Полоска слева
                        Rectangle {
                            width: 3; height: parent.height; radius: 2
                            color: selectedRow === index    ? cAccent
                                 : modelData.st === "expel" ? cRed
                                 : modelData.st === "debt"  ? "#ffa726"
                                 :                            "#4caf50"
                        }

                        Row {
                            anchors.fill: parent
                            DCell { width:40;  cellText:(index+1).toString(); cellColor:cDim }
                            DCell { width:82;  cellText:modelData.grp; cellColor:cNavy; bold:true }
                            DCell { width:158; cellText:modelData.nm;  cellColor:cText }

                            Item {
                                width:155; height:38
                                Rectangle { anchors.right:parent.right; width:1; height:parent.height; color:cBorder }
                                Row {
                                    anchors.verticalCenter:parent.verticalCenter
                                    anchors.left:parent.left; anchors.leftMargin:8; spacing:4
                                    Repeater {
                                        model:[modelData.w1,modelData.w2,modelData.w3,modelData.w4,modelData.w5]
                                        delegate: Badge {
                                            badgeText: modelData.toString()
                                            badgeColor: modelData<3?"red":modelData===5?"green":"blue"
                                        }
                                    }
                                }
                            }
                            DCell {
                                width:78; bold:true; cellText:modelData.avgW
                                cellColor: parseFloat(modelData.avgW)<3    ? cRed
                                         : parseFloat(modelData.avgW)>=4.5 ? cGreen : cOrange
                            }
                            Item {
                                width:155; height:38
                                Rectangle { anchors.right:parent.right; width:1; height:parent.height; color:cBorder }
                                Row {
                                    anchors.verticalCenter:parent.verticalCenter
                                    anchors.left:parent.left; anchors.leftMargin:8; spacing:4
                                    Repeater {
                                        model:[modelData.s1,modelData.s2,modelData.s3,modelData.s4,modelData.s5]
                                        delegate: Badge {
                                            badgeText: modelData.toString()
                                            badgeColor: modelData<3?"red":modelData===5?"green":"blue"
                                        }
                                    }
                                }
                            }
                            DCell {
                                width:78; bold:true; cellText:modelData.avgS
                                cellColor: parseFloat(modelData.avgS)<3    ? cRed
                                         : parseFloat(modelData.avgS)>=4.5 ? cGreen : cOrange
                            }
                            Item {
                                width:108; height:38
                                Rectangle { anchors.right:parent.right; width:1; height:parent.height; color:cBorder }
                                Badge {
                                    anchors.centerIn:parent
                                    badgeText:  modelData.st==="expel"?"ОТЧИСЛИТЬ":modelData.st==="debt"?"ДОЛГ":"OK"
                                    badgeColor: modelData.st==="expel"?"red":modelData.st==="debt"?"yellow":"green"
                                }
                            }
                            Item {
                                width:82; height:38
                                Row {
                                    anchors.centerIn:parent; spacing:4
                                    AppButton {
                                        text:"✏"; implicitWidth:30; leftPadding:4; rightPadding:4
                                        onClicked: {
                                            selectedRow = index
                                            // modelData.idx — реальный индекс в m_students
                                            // нужен потому что после фильтрации
                                            // index в ListView ≠ индекс в векторе
                                            studentPopup.openEdit(modelData.idx)
                                        }
                                    }
                                    AppButton {
                                        text:"✕"; variant:"danger"; implicitWidth:30; leftPadding:4; rightPadding:4
                                        onClicked: {
                                            if (typeof studentBE !== "undefined") {
                                                studentBE.deleteStudent(modelData.idx)
                                                if (selectedRow === index) selectedRow = -1
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        Rectangle { anchors.bottom:parent.bottom; width:parent.width; height:1; color:cBorder }
                    }
                }
            }
        }

        // ── Панель сортировки ─────────────────────────────────
        AppToolBar {
            Layout.fillWidth: true
            Text { text:"Сортировка:"; color:cDim; font.pixelSize:12; font.family:"Arial" }
            AppButton {
                text: "По ФИО " + (typeof studentBE !== "undefined"
                                   && studentBE.sortColumn==="name"
                                   ? (studentBE.sortAsc?"↑":"↓") : "↕")
                onClicked: { if (typeof studentBE !== "undefined") studentBE.sortByName() }
            }
            AppButton {
                text: "По группе " + (typeof studentBE !== "undefined"
                                      && studentBE.sortColumn==="group"
                                      ? (studentBE.sortAsc?"↑":"↓") : "↕")
                onClicked: { if (typeof studentBE !== "undefined") studentBE.sortByGroup() }
            }
            AppButton {
                text: "По ср. баллу " + (typeof studentBE !== "undefined"
                                         && studentBE.sortColumn==="avg"
                                         ? (studentBE.sortAsc?"↑":"↓") : "↕")
                onClicked: { if (typeof studentBE !== "undefined") studentBE.sortByAvgDesc() }
            }
            ToolSep {}
            Rectangle { width:10;height:10;radius:5; color:cRed; Layout.alignment:Qt.AlignVCenter }
            Text { text:"Отчислить:"; color:cDim; font.pixelSize:12; font.family:"Arial" }
            Text { text: typeof studentBE!=="undefined" ? studentBE.expelCount.toString() : "2"
                   color:cRed; font.pixelSize:13; font.weight:Font.Bold; font.family:"Arial" }
            Rectangle { width:10;height:10;radius:5; color:"#ffa726"; Layout.alignment:Qt.AlignVCenter }
            Text { text:"Должники:"; color:cDim; font.pixelSize:12; font.family:"Arial" }
            Text { text: typeof studentBE!=="undefined" ? studentBE.debtCount.toString() : "2"
                   color:"#e65100"; font.pixelSize:13; font.weight:Font.Bold; font.family:"Arial" }
            Rectangle { width:10;height:10;radius:5; color:"#4caf50"; Layout.alignment:Qt.AlignVCenter }
            Text { text:"OK:"; color:cDim; font.pixelSize:12; font.family:"Arial" }
            Text { text: typeof studentBE!=="undefined" ? studentBE.okCount.toString() : "2"
                   color:cGreen; font.pixelSize:13; font.weight:Font.Bold; font.family:"Arial" }
            Item { Layout.fillWidth:true }
        }
    }
}
