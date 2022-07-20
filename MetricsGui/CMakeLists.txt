cmake_minimum_required(VERSION 3.11.0)

add_library(MetricsGui)

if(MSVC)
	target_compile_options(MetricsGui PRIVATE /W4)
	
	if(CMAKE_BUILD_TYPE STREQUAL "Debug")
		# debug flags and things
		#target_compile_options(MetricsGui PRIVATE /DEBUG:FULL /Zi /EHsc)
		target_compile_options(MetricsGui PRIVATE /fsanitize=address /Zi /EHsc)
		target_link_options(MetricsGui PRIVATE /DEBUG:FULL)
	else()
		# todo: add optimization flags or w/e
		target_compile_options(MetricsGui PRIVATE /O2)
	endif()
else()
	target_compile_options(DecemberBliss PRIVATE -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion )
	if(CMAKE_BUILD_TYPE STREQUAL "Debug")
		# debug flags and things
	else()
		# todo: add optimization flags or w/e
	endif()
endif()

target_include_directories(MetricsGui 
    INTERFACE metrics_gui/include/metrics_gui
    INTERFACE metrics_gui/portable/)

target_sources(MetricsGui PRIVATE
    source/metrics_gui.cpp 
    source/ProggyTiny.cpp)

target_link_libraries(MetricsGui imgui-1.85)

