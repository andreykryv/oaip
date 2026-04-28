QT += core gui widgets

CONFIG += c++17 c++1z
TARGET   = lab3_recursion
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++17

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h