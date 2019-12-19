cmake_minimum_required(VERSION 3.16.0)
project(raygui VERSION 2.6)

add_library(raygui INTERFACE)

# if(NOT CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
#     #don't bother using projects with warnings
#     if(MSVC)
#         target_compile_options(raygui INTERFACE /W0)
#     else()
#         target_compile_options(raygui INTERFACE -w)
#     endif()
# endif()

target_include_directories(raygui INTERFACE .)
