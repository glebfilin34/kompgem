cmake_minimum_required(VERSION 3.16)
project(RotatingSphere3D)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(sphere_algorithms STATIC sphere_algorithms.cpp)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui OpenGLWidgets)

qt6_wrap_cpp(MOC_SOURCES sphere_visualization.h)

add_executable(sphere_3d main.cpp sphere_visualization.cpp ${MOC_SOURCES})
target_link_libraries(sphere_3d Qt6::Core Qt6::Widgets Qt6::Gui Qt6::OpenGLWidgets sphere_algorithms)
