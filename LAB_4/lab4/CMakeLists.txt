cmake_minimum_required(VERSION 3.16)
project(Lab4 VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(Lab4
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/vector.h
    src/pair.h
    src/sorting.h
)

target_link_libraries(Lab4 PRIVATE Qt6::Widgets)

target_compile_options(Lab4 PRIVATE
    -Wall -Wextra -O2
)
