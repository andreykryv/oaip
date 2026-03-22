# LabWork2 — Qt 6 QML Visual

Готовый визуал для Qt Design Studio / Qt Creator.
Подключите C++ backend — и всё работает.

## Структура проекта

```
LabWork2/
├── CMakeLists.txt
├── src/
│   ├── main.cpp
│   └── backend/
│       ├── datecontroller.h        <- заглушки для Задания 1
│       ├── studentcontroller.h     <- заглушки для Задания 2
│       └── expressioncontroller.h  <- заглушки для Задания 3/4
└── qml/
    ├── Main.qml                    <- окно + TabBar + StatusBar
    ├── DatesPage.qml               <- Задание 1: Даты
    ├── DeanPage.qml                <- Задание 2: Деканат
    ├── ExprPage.qml                <- Задание 3: Выражения
    ├── TypeCheckPage.qml           <- Задание 4: Типы
    └── components/
        ├── AppButton.qml
        ├── AppInput.qml
        ├── AppComboBox.qml
        ├── AppGroupBox.qml
        ├── AppToolBar.qml
        ├── AppFileDialog.qml
        ├── AppErrorDialog.qml
        ├── Badge.qml
        ├── DateCell.qml / DCell.qml
        ├── ToolSep.qml
        └── StatusItem.qml / StatusSep.qml
```

## Как открыть в Qt Design Studio

1. Откройте Qt Design Studio
2. File -> Open Project -> выберите CMakeLists.txt
3. В Navigator слева раскройте qml/
4. Двойной клик на любой *Page.qml — откроется визуальный редактор

## Как подключить C++ backend

### Шаг 1 — Реализуйте заглушки

Откройте src/backend/datecontroller.h — там перечислены
все нужные слоты и сигналы. Создайте datecontroller.cpp:

    void DateController::openFile(const QString& path) {
        // читать файл, заполнить m_dates, emit dateModelChanged()
    }
    QString DateController::nextDay(const QString& dateStr) const {
        // вернуть следующий день
    }

### Шаг 2 — Зарегистрируйте в main.cpp

Раскомментируйте блок в src/main.cpp:

    DateController       dateCtrl;
    StudentController    studentCtrl;
    ExpressionController exprCtrl;
    engine.rootContext()->setContextProperty("dateBE",    &dateCtrl);
    engine.rootContext()->setContextProperty("studentBE", &studentCtrl);
    engine.rootContext()->setContextProperty("exprBE",    &exprCtrl);

### Шаг 3 — Подключите кнопки в QML

В каждой *Page.qml кнопки уже размечены комментариями.
Пример для DatesPage.qml:

    AppButton {
        text: "Открыть файл"
        onClicked: openDlg.open()
    }
    AppFileDialog {
        id: openDlg
        mode: "open"
        onAccepted: dateBE.openFile(selectedFile)
    }

## Требования

- Qt 6.4+
- Qt Quick Controls 2
- Qt Quick Dialogs
- C++17
