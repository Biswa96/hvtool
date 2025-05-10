cmake_minimum_required(VERSION 3.12.0)
project(hvtool)
enable_language(C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(hvtool_srcs
    src/main.cpp
    src/network.cpp
    src/system.cpp
    src/util.cpp
)

add_executable(hvtool ${hvtool_srcs})

target_include_directories(hvtool PRIVATE
    json/include
    wil/include
)

target_link_libraries(hvtool PRIVATE
    computecore
    computenetwork
    rpcrt4
    ws2_32
)
