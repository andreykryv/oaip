cmake_minimum_required(VERSION 3.16)
project(Lab5 VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)  # <-- обязательно для обработки Q_OBJECT

find_package(Qt6 REQUIRED COMPONENTS Widgets Core Gui)

add_subdirectory(stringlib)

add_executable(Lab5App
    main.cpp
    mainwindow.h mainwindow.cpp
    bitset.h bitset.cpp
    keyboardtrainer.h keyboardtrainer.cpp
    stringdemo.h stringdemo.cpp
    bitsetdemo.h bitsetdemo.cpp
)

target_link_libraries(Lab5App PRIVATE Qt6::Widgets Qt6::Core Qt6::Gui stringlib)
target_include_directories(Lab5App PRIVATE ${CMAKE_SOURCE_DIR}/stringlib)