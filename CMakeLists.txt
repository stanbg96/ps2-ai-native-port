cmake_minimum_required(VERSION 3.10)
project(ps2_engine_native CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

file(GLOB_RECURSE SOURCES 
    "src/graphics/*.cpp"
    "src/input/*.cpp"
    "src/core/*.cpp"
    "src/recompiler/mips_r5900_core.cpp"
    "src/recompiler/elf_loader.cpp"
    "src/recompiler/iso_reader.cpp"
    "src/recompiler/trace_recorder.cpp"
    "src/recompiler/turbo_translator.cpp"
)

list(FILTER SOURCES EXCLUDE REGEX ".*main\\.cpp$")
list(FILTER SOURCES EXCLUDE REGEX ".*crash_translated\\.cpp$")

add_library(ps2_engine_native SHARED ${SOURCES})

# Свързване с хардуерните библиотеки на Android за дисплей и 3D графика
target_link_libraries(ps2_engine_native
    android
    EGL
    GLESv3
    log
)
