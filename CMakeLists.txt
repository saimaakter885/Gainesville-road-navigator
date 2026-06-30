cmake_minimum_required(VERSION 3.22)
project(GainesvilleRoadNavigator)

set(CMAKE_CXX_STANDARD 14)
set(GCC_COVERAGE_COMPILE_FLAGS "-Wall -Werror")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")

include(FetchContent)
FetchContent_Declare(
        json
        URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz
)
FetchContent_MakeAvailable(json)
include_directories(src)

add_executable(GainesvilleRoadNavigator
        src/main.cpp
        src/parser.cpp
        src/graph.cpp
        src/dijkstra.cpp
        src/astar.cpp
        src/distance.cpp
        src/minHeap.cpp
)

target_link_libraries(GainesvilleRoadNavigator PRIVATE nlohmann_json::nlohmann_json)