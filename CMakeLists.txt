cmake_minimum_required(VERSION 3.10)
project(ps2_engine_native CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

# Оптимизация за бърза компилация на големия 47 MB файл
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -march=armv8-a")

file(GLOB_RECURSE SOURCES 
    "src/recompiler/*.cpp"
    "src/input/*.cpp"
    "src/graphics/*.cpp"
    "src/core/*.cpp"
)

list(FILTER SOURCES EXCLUDE REGEX ".*main\\.cpp$")

add_library(ps2_engine_native SHARED ${SOURCES})
