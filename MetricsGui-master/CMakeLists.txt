cmake_minimum_required(VERSION 3.11.0)
project(MetricsGui VERSION 0.1.0 LANGUAGES C CXX)

add_library(MetricsGui)

# add_subdirectory(../cimgui/imgui)

target_include_directories(MetricsGui PUBLIC 
    metrics_gui/include/metrics_gui.h)

target_sources(MetricsGui PRIVATE
    metrics_gui/source/metrics_gui.cpp 
    metrics_gui/source/ProggyTiny.cpp)

# target_link_libraries(imgui)