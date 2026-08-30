cmake_minimum_required(VERSION 3.10)
project(ps2_engine_native CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(ps2_engine_native SHARED src/core/native_engine.cpp)

target_link_libraries(ps2_engine_native
    android
    EGL
    GLESv3
    log
)
