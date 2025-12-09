cmake_minimum_required(VERSION 3.16)
project(ComplexPolygonOps)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(polygon_bool_algorithms STATIC polygon_bool_algorithms.cpp)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

qt6_wrap_cpp(MOC_SOURCES polygon_bool_visualization.h)

add_executable(polygon_bool_app main.cpp polygon_bool_visualization.cpp ${MOC_SOURCES})
target_link_libraries(polygon_bool_app Qt6::Core Qt6::Widgets polygon_bool_algorithms)
