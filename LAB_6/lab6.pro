QT += core gui widgets

CONFIG   += c++17
TARGET    = lab6_files
TEMPLATE  = app

QMAKE_CXXFLAGS += -std=c++17

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h
