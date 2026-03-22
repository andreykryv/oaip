cmake_minimum_required(VERSION 3.16)
project(CarApp3D LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets OpenGL OpenGLWidgets)
find_package(OpenGL REQUIRED)

# Assimp (system)
find_library(ASSIMP_LIB assimp REQUIRED)
find_path(ASSIMP_INCLUDE assimp/Importer.hpp REQUIRED)

add_executable(CarApp3D
    main.cpp
    shape3d.h
    movingrectangle.h
    movingrectangle.cpp
    glmesh.h
    glmesh.cpp
    glscene.h
    glscene.cpp
    glwidget.h
    glwidget.cpp
    mainwindow.h
    mainwindow.cpp
    resources.qrc
)

target_include_directories(CarApp3D PRIVATE ${ASSIMP_INCLUDE})

target_link_libraries(CarApp3D PRIVATE
    Qt6::Widgets
    Qt6::OpenGL
    Qt6::OpenGLWidgets
    OpenGL::GL
    OpenGL::GLU
    ${ASSIMP_LIB}
)
