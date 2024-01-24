cmake_minimum_required(VERSION 3.11.0)

add_library(imgui-1.85 STATIC ${CMAKE_CURRENT_SOURCE_DIR}/imgui.cpp)
target_link_libraries(imgui-1.85 PUBLIC raylib)
target_sources(imgui-1.85 PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui_draw.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui_widgets.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui_tables.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui_demo.cpp)
target_include_directories(imgui-1.85 INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# target_sources(imgui PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/misc/freetype/imgui_freetype.cpp)
# target_sources(imgui PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/misc/fonts/binary_to_compressed_c.cpp)
# target_sources(imgui PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/misc/cpp/imgui_stdlib.cpp)
