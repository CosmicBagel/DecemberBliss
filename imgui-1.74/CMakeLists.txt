cmake_minimum_required(VERSION 3.11.0)
project(imgui VERSION 1.74)

add_library(imgui)

target_sources(imgui PRIVATE imgui.cpp imgui_draw.cpp 
target_sources(imgui PRIVATE freetype/imgui_freetype.cpp)
target_sources(imgui PRIVATE fonts/binary_to_compressed_c.cpp)
target_sources(imgui PRIVATE cpp/imgui_stdlib.cpp)
target_sources(imgui PUBLIC imgui.h)