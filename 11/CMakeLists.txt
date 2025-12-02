cmake_minimum_required(VERSION 3.16)
project(PointInConvexHull)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(point_in_hull_algorithms STATIC point_in_hull_algorithms.cpp)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

qt6_wrap_cpp(MOC_SOURCES point_in_hull_visualization.h)

add_executable(point_in_hull_app main.cpp point_in_hull_visualization.cpp ${MOC_SOURCES})
target_link_libraries(point_in_hull_app Qt6::Core Qt6::Widgets point_in_hull_algorithms)
