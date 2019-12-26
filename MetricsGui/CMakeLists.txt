cmake_minimum_required(VERSION 3.11.0)

add_library(MetricsGui)

target_include_directories(MetricsGui 
    INTERFACE metrics_gui/include/metrics_gui
    INTERFACE metrics_gui/portable/)

target_sources(MetricsGui PRIVATE
    source/metrics_gui.cpp 
    source/ProggyTiny.cpp)

target_link_libraries(MetricsGui imgui)