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
    # Also set the c standard desired, this should also only be done at a global level
    # as you can't mix ABI standards, everything must compile under one standard
    if(MSVC)
        target_compile_options(DecemberBliss PRIVATE /W4)
        
        if (CMAKE_BUILD_TYPE STREQUAL "Debug")
            target_compile_options(DecemberBliss PRIVATE /fsanitize=address)
        else()
            # todo: add optimization flags or w/e
        endif()
    else()
        target_compile_options(DecemberBliss PRIVATE -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion )
        if (CMAKE_BUILD_TYPE STREQUAL "Debug")
            target_compile_options(DecemberBliss PRIVATE -fsanitize=address)
        else()
            # todo: add optimization flags or w/e
        endif()
    endif()

    # this will give us a soft enforcement of the c11 standard, project won't error out, but 
    # because of our warning flags, we will still be told when we're using non-standard c    
    # mixing c / c++ standard can cause weird ABI issues when linking
    target_compile_features(DecemberBliss PRIVATE c_std_11 cxx_std_20)

endif()

### Python is an external dependency, and must be installed before hand
#   x86 and x64 for the respective build target
#set(Python3_USE_STATIC_LIBS TRUE)
#find_package(Python3 3.8 REQUIRED COMPONENTS Development)

### Included dependencies' source code
add_subdirectory(raylib-4.0.0/)
add_subdirectory(imgui-1.85/)
add_subdirectory(MetricsGui/)

### Declare dependencies
target_link_libraries(DecemberBliss PRIVATE
    raylib imgui-1.85 MetricsGui
    )

### Project source
target_sources(DecemberBliss PRIVATE 
    src/main.cpp src/LogGui.cpp src/DevUI.cpp src/bliss_app.cpp src/log_router.cpp)
target_include_directories(DecemberBliss PRIVATE src)

### Get Git info for buildInfo.h
# Get the current working branch
execute_process(
        COMMAND git rev-parse --abbrev-ref HEAD
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_BRANCH
        OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Get the latest abbreviated commit hash of the working branch
execute_process(
        COMMAND git log -1 --format=%h
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_COMMIT_HASH
        OUTPUT_STRIP_TRAILING_WHITESPACE
)

### Pass name and version to code via defines (as opposed to having it in multiple places)
configure_file(build_info.h.in build_info.h)
#the resulting buildInfo.h will be put into the build results, so we need to include that
target_include_directories(DecemberBliss PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

### Copy resources folder to build destination
file(COPY ${CMAKE_SOURCE_DIR}/resources DESTINATION ${CMAKE_BINARY_DIR})

### Copy python runtime dependencies to build destination
#file(COPY ${CMAKE_SOURCE_DIR}/python-embeded/${BLISS_ARCH}/ DESTINATION ${CMAKE_BINARY_DIR})