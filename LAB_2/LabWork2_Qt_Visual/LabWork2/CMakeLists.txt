cmake_minimum_required(VERSION 3.16)
project(LabWork2App VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Qml Quick QuickControls2)

qt_policy(SET QTP0001 NEW)

# ── Исполняемый файл ─────────────────────────────────────────
qt_add_executable(LabWork2App
    src/main.cpp

    # ── Задание 1: Даты ──────────────────────────────────────
    src/backend/Date.h
    src/backend/Date.cpp
    src/backend/DateController.h
    src/backend/DateController.cpp

    # ── Задание 2: Деканат ───────────────────────────────────
    src/backend/Student.h
    src/backend/Student.cpp
    src/backend/StudentController.h
    src/backend/StudentController.cpp

    # ── Задания 3+4: Выражения ───────────────────────────────
    src/backend/Expression.h
    src/backend/Expression.cpp
    src/backend/ExpressionController.h
    src/backend/ExpressionController.cpp
)

# ── QML модуль ───────────────────────────────────────────────
qt_add_qml_module(LabWork2App
    URI LabWork2
    VERSION 1.0
    RESOURCE_PREFIX "/qt/qml"
    QML_FILES
        Main.qml
        Screen01.qml
        DatesPage.qml
        DeanPage.qml
        ExprPage.qml
        TypeCheckPage.qml
        AppTabButton.qml
        AppButton.qml
        AppInput.qml
        AppComboBox.qml
        AppGroupBox.qml
        AppToolBar.qml
        AppFileDialog.qml
        AppErrorDialog.qml
        Badge.qml
        DateCell.qml
        DCell.qml
        ToolSep.qml
        StatusItem.qml
        StatusSep.qml
        MenuButton.qml
)

# ── Зависимости ──────────────────────────────────────────────
target_link_libraries(LabWork2App PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
    Qt6::QuickControls2
    Qt6::Widgets
)

# Include path для backend заголовков
target_include_directories(LabWork2App PRIVATE src/backend)
