# ────────────────────────────────────────────────────────────────────────
#  Лабораторная работа №3 — Рекурсия (Qt 6 Modern)
#  C++17 | Qt 6.x | Графическое приложение с современным UI
# ────────────────────────────────────────────────────────────────────────

cmake_minimum_required(VERSION 3.16)
project(LAB3_Recursion VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Поиск Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

qt_standard_project_setup()

# Исходные файлы
set(SOURCES
    main.cpp
    mainwindow.cpp
)

# Заголовочные файлы
set(HEADERS
    mainwindow.h
)

# Создание исполняемого файла
add_executable(lab3_recursion
    ${SOURCES}
    ${HEADERS}
)

# Подключение модулей Qt
target_link_libraries(lab3_recursion PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)

# Установка целевой папки для бинарника
set_target_properties(lab3_recursion PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
