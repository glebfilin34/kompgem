cmake_minimum_required(VERSION 3.16)
project(BezierCurves)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(bezier_algorithms STATIC bezier_algorithms.cpp)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

qt6_wrap_cpp(MOC_SOURCES bezier_visualization.h)

add_executable(bezier_app main.cpp bezier_visualization.cpp ${MOC_SOURCES})
target_link_libraries(bezier_app Qt6::Core Qt6::Widgets bezier_algorithms)
