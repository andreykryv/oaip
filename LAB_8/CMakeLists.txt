cmake_minimum_required(VERSION 3.16)
project(Lab8 VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Core Gui)

add_executable(Lab8App
    main.cpp
    mainwindow.h
    mainwindow.cpp
    bst.h
    bstviswidget.h
    bstviswidget.cpp
    arrayheap.h
    arrayheap_inst.cpp
    arrayheapviswidget.h
    arrayheapviswidget.cpp
    listheap.h
    listheap_inst.cpp
    listheapviswidget.h
    listheapviswidget.cpp
    hashtable.h
    hashtableviswidget.h
    hashtableviswidget.cpp
    treemap.h
)

target_link_libraries(Lab8App PRIVATE Qt6::Widgets Qt6::Core Qt6::Gui)
target_include_directories(Lab8App PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
