QT += core gui widgets

CONFIG += c++17
TARGET = shapes3d
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    renderer3d.cpp \
    shape.cpp \
    circle_shape.cpp \
    polygon_shape.cpp \
    triangle_shape.cpp \
    rectangle_shape.cpp \
    square_shape.cpp \
    rhombus_shape.cpp \
    star_shape.cpp \
    hexagon_shape.cpp \
    custom_shape.cpp

HEADERS += \
    mainwindow.h \
    renderer3d.h \
    shape.h \
    circle_shape.h \
    polygon_shape.h \
    triangle_shape.h \
    rectangle_shape.h \
    square_shape.h \
    rhombus_shape.h \
    star_shape.h \
    hexagon_shape.h \
    custom_shape.h
