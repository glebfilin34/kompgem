cmake_minimum_required(VERSION 3.16)
project(DelaunayTriangulation)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(delaunay_algorithms STATIC delaunay_algorithms.cpp)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

qt6_wrap_cpp(MOC_SOURCES delaunay_visualization.h)

add_executable(delaunay_app main.cpp delaunay_visualization.cpp ${MOC_SOURCES})
target_link_libraries(delaunay_app Qt6::Core Qt6::Widgets delaunay_algorithms)
