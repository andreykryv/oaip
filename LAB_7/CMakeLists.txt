cmake_minimum_required(VERSION 3.16)
project(Lab7 VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Core Gui Charts)

add_executable(Lab7App
    src/main.cpp
    src/mainwindow.h
    src/mainwindow.cpp
    src/task1_queue.h
    src/task2_bst.h
    src/task3_hash.h
    src/task4_openaddr.h
    src/task5_deque.h
    src/animwidget.h
)

target_link_libraries(Lab7App PRIVATE
    Qt6::Widgets Qt6::Core Qt6::Gui Qt6::Charts)
target_include_directories(Lab7App PRIVATE src)
