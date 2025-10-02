cmake_minimum_required(VERSION 3.10)
project(flightRouteOptimization)

set(CMAKE_CXX_STANDARD 17)

# Add the include directory
include_directories(${PROJECT_SOURCE_DIR}/include)

# Add source files
set(SOURCES
    src/main.cpp
    src/Graph.cpp
    src/Airport.cpp
    src/Route.cpp
)

add_executable(flightRouteOptimization ${SOURCES})
