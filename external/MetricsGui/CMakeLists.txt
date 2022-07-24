cmake_minimum_required(VERSION 3.11.0)

add_library(MetricsGui)

if(MSVC)
	target_compile_options(MetricsGui PRIVATE /W4 /experimental:external /external:anglebrackets /external:W0 /external:I"../../external" /EHsc)

	if(CMAKE_BUILD_TYPE STREQUAL "Sanitize")
		# /Zi produces a separate PDB file (for debugging)
		# /EHsc enables (s) stack unwinding and (c) "extern" C functions never throw exceptions
		# /fsanitize=address enables the address sanitizer
		target_compile_options(MetricsGui PRIVATE /fsanitize=address /Zi /EHsc)

		# give us the full debug output
		target_link_options(MetricsGui PRIVATE /DEBUG:FULL)
	elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
		# debug flags and things
		# /Zi produces a separate PDB file (for debugging)
		# /EHsc enables (s) stack unwinding and (c) "extern" C functions never throw exceptions
		target_compile_options(MetricsGui PRIVATE /Zi /EHsc)
		target_link_options(MetricsGui PRIVATE /DEBUG:FULL)
	else()
		# todo: add optimization flags or w/e
		target_compile_options(MetricsGui PRIVATE /O2)
	endif()
else()
	target_compile_options(MetricsGui PRIVATE -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion)

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
