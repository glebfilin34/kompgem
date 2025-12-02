cmake_minimum_required(VERSION 3.16)
project(PolygonOperations)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(polygon_ops_algorithms STATIC polygon_ops_algorithms.cpp)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

qt6_wrap_cpp(MOC_SOURCES polygon_ops_visualization.h)

add_executable(polygon_ops_app main.cpp polygon_ops_visualization.cpp ${MOC_SOURCES})
target_link_libraries(polygon_ops_app Qt6::Core Qt6::Widgets polygon_ops_algorithms)
