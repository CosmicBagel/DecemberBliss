# Trying to stick with Effective Modern CMake
# https://gist.github.com/mbinna/c61dbb39bca0e4fb7d1f73b0d66a4fd1
# Use a 'find module' for third party libraries not built with CMake

cmake_minimum_required(VERSION 3.11.0)
project(DecemberBliss VERSION 0.1.0 LANGUAGES C CXX)

include(BlissBuildHelpers.cmake)

add_executable(DecemberBliss)

# ie if this is the top most level script
# these options should only be set if not being used by another project
# don't mess with compiler flags unless we're the top level script
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    if(MSVC)
        # the defaults
        # CMAKE_CXX_FLAGS:STRING=/DWIN32 /D_WINDOWS /W3 /GR /EHsc
        # CMAKE_CXX_FLAGS_DEBUG:STRING=/MDd /Zi /Ob0 /Od /RTC1
        # CMAKE_CXX_FLAGS_RELEASE:STRING=/MD /O2 /Ob2 /DNDEBUG
        # CMAKE_CXX_FLAGS_SANITIZE:STRING=
        # CMAKE_C_FLAGS:STRING=/DWIN32 /D_WINDOWS /W3
        # CMAKE_C_FLAGS_DEBUG:STRING=/MDd /Zi /Ob0 /Od /RTC1
        # CMAKE_C_FLAGS_RELEASE:STRING=/MD /O2 /Ob2 /DNDEBUG
        # CMAKE_C_FLAGS_SANITIZE:STRING=
        # reset the flags (note these affect all code including external)
        set(CMAKE_CXX_FLAGS          "/EHsc /GR /DWIN32 /D_WINDOWS" CACHE STRING "" FORCE)
        set(CMAKE_CXX_FLAGS_DEBUG    "/Zi /Ob0 /Od" CACHE STRING "" FORCE)
        set(CMAKE_CXX_FLAGS_SANITIZE "/Zi /Ob0 /Od /fsanitize=address" CACHE STRING "" FORCE)
        set(CMAKE_CXX_FLAGS_RELEASE  "/O2 /DNDEBUG" CACHE STRING "" FORCE)
        set(CMAKE_C_FLAGS            "/DWIN32 /D_WINDOWS" CACHE STRING "" FORCE)
        set(CMAKE_C_FLAGS_DEBUG      "/Zi /Ob0 /Od" CACHE STRING "" FORCE)
        set(CMAKE_C_FLAGS_SANITIZE   "/Zi /Ob0 /Od /fsanitize=address" CACHE STRING "" FORCE)
        set(CMAKE_C_FLAGS_RELEASE    "/O2 /DNDEBUG" CACHE STRING "" FORCE)

        # the defaults
        # CMAKE_EXE_LINKER_FLAGS:STRING=/machine:x64
        # CMAKE_EXE_LINKER_FLAGS_DEBUG:STRING=/debug /INCREMENTAL
        # CMAKE_EXE_LINKER_FLAGS_MINSIZEREL:STRING=/INCREMENTAL:NO
        # CMAKE_EXE_LINKER_FLAGS_RELEASE:STRING=/INCREMENTAL:NO
        # CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO:STRING=/debug /INCREMENTAL
        # CMAKE_EXE_LINKER_FLAGS_SANITIZE:STRING=
        set(CMAKE_EXE_LINKER_FLAGS_DEBUG "/DEBUG:FULL /INCREMENTAL /PROFILE" CACHE STRING "" FORCE)
        set(CMAKE_EXE_LINKER_FLAGS_SANITIZE "/DEBUG:FULL /INCREMENTAL" CACHE STRING "" FORCE)

        target_compile_options(DecemberBliss PRIVATE /W4 /analyze)

        if(CMAKE_BUILD_TYPE STREQUAL "Sanitize")
            target_compile_options(DecemberBliss PRIVATE /RTC1)
        elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
            target_compile_options(DecemberBliss PRIVATE /RTC1)
        else()
            # "Release" flags
        endif()
    else()
        target_compile_options(DecemberBliss PRIVATE -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion)

        if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        # debug flags and things
        else()
            # todo: add optimization flags or w/e
        endif()
    endif()

    # this will give us a soft enforcement of the c11 standard and cpp17, project won't error out, 
    # but because of our warning flags, we will still be told when we're using non-standard c
    # mixing c / c++ standard can cause weird ABI issues when linking
    target_compile_features(DecemberBliss PRIVATE c_std_11 cxx_std_17)
endif()

# ## Included dependencies' source code
add_subdirectory(external/)

# ## Declare dependencies
target_link_libraries(DecemberBliss PRIVATE
    raylib imgui-1.85 MetricsGui
)

# ## Project source
target_sources(DecemberBliss PRIVATE
    src/main.cpp src/log_gui.cpp src/dev_ui.cpp src/bliss_app.cpp
    src/log_router.cpp src/perf_timer.cpp src/entity.cpp
    src/entity_memory_pool.cpp src/entity_manager.cpp 
    src/imgui_impl_raylib.cpp
)
target_include_directories(DecemberBliss PRIVATE src)

# ## Get Git info for buildInfo.h
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

# ## Pass name and version to code via defines (as opposed to having it in multiple places)
configure_file(build_info.hpp.in build_info.hpp)

# the resulting buildInfo.h will be put into the build results, so we need to include that
target_include_directories(DecemberBliss PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

# ## Copy resources folder to build destination
file(COPY ${CMAKE_SOURCE_DIR}/data DESTINATION ${CMAKE_BINARY_DIR})
