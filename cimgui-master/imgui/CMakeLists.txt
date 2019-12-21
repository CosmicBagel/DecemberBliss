cmake_minimum_required(VERSION 3.11.0)
project(imgui VERSION 1.74)

add_library(imgui STATIC ${CMAKE_CURRENT_SOURCE_DIR}/imgui.cpp)
# target_sources(imgui PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/imgui_draw.cpp)
target_sources(imgui INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/imgui.h)


# target_sources(imgui PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/misc/freetype/imgui_freetype.cpp)
# target_sources(imgui PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/misc/fonts/binary_to_compressed_c.cpp)
# target_sources(imgui PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/misc/cpp/imgui_stdlib.cpp)
