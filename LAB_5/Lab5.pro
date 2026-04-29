QT += widgets

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    bitset.cpp \
    keyboardtrainer.cpp \
    stringdemo.cpp \
    bitsetdemo.cpp \
    stringlib/stringlib.cpp

HEADERS += \
    mainwindow.h \
    bitset.h \
    keyboardtrainer.h \
    stringdemo.h \
    bitsetdemo.h \
    stringlib/stringlib.h \
    stringlib/stringlib_global.h

FORMS +=

INCLUDEPATH += $$PWD/stringlib

CONFIG += c++17
