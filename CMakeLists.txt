# Trying to stick with Effective Modern CMake 
#   https://gist.github.com/mbinna/c61dbb39bca0e4fb7d1f73b0d66a4fd1
# Use a 'find module' for third party libraries not built with CMake
# 

cmake_minimum_required(VERSION 3.11.0)
project(DecemberBliss VERSION 0.1.0 LANGUAGES C CXX)

include(BlissBuildHelpers.cmake)

add_executable(DecemberBliss)

#ie if this is the top most level script
#   these options should only be set if not being used by another project
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)

    # Add warnings as global options, and ONLY at the root level of the project
    # Also set the c standrd desired, this should also only be done at a global level
    # as you can't mix ABI standards, everything must compile under one standard
    if(MSVC)
        target_compile_options(DecemberBliss PRIVATE /W4)
    else()
        target_compile_options(DecemberBliss PRIVATE -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion)
    endif()

    # this will give us a soft enforcement of the c11 standard, project won't error out, but 
    # because of our warning flags, we will still be told when we're using non-standard c    
    # mixing c / c++ standard can cause weird ABI issues when linking
    target_compile_features(DecemberBliss PRIVATE c_std_11)

endif()

### Included dependencies' source code
add_subdirectory(raylib/)
add_subdirectory(entt-3.2.2/)
add_subdirectory(Chipmunk2D-Chipmunk-7.0.3/)
add_subdirectory(cimgui-master/)

### Declare dependencies
target_link_libraries(DecemberBliss PRIVATE 
    raylib EnTT chipmunk_static cimgui)

### Project source
target_sources(DecemberBliss PRIVATE 
    main.c DevUI.c enttTest.cpp imgui_impl_raylib.c imgui_impl_raylibgl3.c)

### Pass name and version to code via defines (as opposed to having it in multiple places)
configure_file(buildInfo.h.in buildInfo.h)
#the resulting buildInfo.h will be put into the build results, so we need to include that
target_include_directories(DecemberBliss PRIVATE ${CMAKE_CURRENT_BINARY_DIR}) 

