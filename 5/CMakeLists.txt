cmake_minimum_required(VERSION 3.16)
project(ConvexHull)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Algorithms library (pure C++, no Qt)
add_library(convex_hull_algorithms STATIC convex_hull_algorithms.cpp
    convex_hull_algorithms.h
    convex_hull_visualization.h)

# Visualization (Qt)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

qt6_wrap_cpp(MOC_SOURCES convex_hull_visualization.h)

add_executable(convex_hull_app main.cpp convex_hull_visualization.cpp ${MOC_SOURCES})
target_link_libraries(convex_hull_app Qt6::Core Qt6::Widgets convex_hull_algorithms)
