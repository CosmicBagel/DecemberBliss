# Trying to stick with Effective Modern CMake 
#   https://gist.github.com/mbinna/c61dbb39bca0e4fb7d1f73b0d66a4fd1

# use message(STATUS "message info ${variable}") for print statements

#lists are ; separated stings
#CMake variables are NOT environment variables (unlike makefile)

#avoid custom variables in arguments of project commands

#don't use variables, but 
#   set(varName varText) varName and varText are strings, and don't need ""
#   unset(varName) makes a string empty

# don't use add_compile_options(), include_directories()
#       link_directories(), link_libraries()
# don't use file(GLOB), not all build systems can use a glob so unexpected things can happen
#   where cmake doesn't refresh, and a build is compiling weird
#   eg visual studio doesn't support globs, it only supports file lists

# do use
# add_executable(), add_library()
# get_target_property(), set_target_properties()
# get_property(TARGET), set_property(TARGET)
# target_compile_definitions(), target_compile_features(), target_compile_options()
# target_include_directories(), target_link_libraries(), target_sources()

#[[
    A generator expression
    $<IF:$<CONFIG:Debug>,30,10>
    $<> syntax
    not evaluated by command interpreter (it is just a string with $<>)
    evaluated during build system generation
    not supported in all commands
#]]

#[[
    function() and macro()
    when new command replaces an existing command, old one can be accessed 
    with a _prefix

    function(my_command input output)
        # ... stuff
        set(${output} ... PARENT_SCOPE)
    endfunction()

    variables are scoped to the function, unless set with PARENT_SCOPE
    available variables: input, output, ARGC, ARGV, ARGN, ARG0, ARG1,...
#]]

#when using add_subdirectory(DIR), that directory must have its own CMakeLists.txt
#CMakeLists.txt are the entry point for the build system generator
#scriptName.cmake can be executed with `cmake -P scriptName.cmake` (not all commands are supported)
#a module is a scriptName.cmake file located int the CMAKE_MODULE_PATH
#   which can be loaded with include()

# all cmake properties 
#   https://cmake.org/cmake/help/latest/manual/cmake-properties.7.html
# available c compile features
#   https://cmake.org/cmake/help/latest/prop_gbl/CMAKE_C_KNOWN_FEATURES.html
# avialable c++ compile features
#   https://cmake.org/cmake/help/latest/prop_gbl/CMAKE_CXX_KNOWN_FEATURES.html

# avoid custom variables in the arguments of project commands
# do not use file(GLOB) in project
# Don't abouse requirements, Eg -Wall is not a requirement to build a project

#[[
todo
- remove unwanted warnings from raylib and its dependencies
    (only warnings from DecemberBliss are wanted, not its dependencies)
- Project name & version injected into c code
- Project linting / static analyzing
- Enforce c standard 
#]]

cmake_minimum_required(VERSION 3.16.0)
project(DecemberBliss VERSION 0.1.0 LANGUAGES C)

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
        target_compile_options(DecemberBliss PRIVATE -Wall -Wextra -Wpedantic)
    endif()

    # this will give us a soft enforcement of the c11 standard, project won't error out, but 
    # because of our warning flags, we will still be told when we're using non-standard c    
    # mixing c / c++ standard can cause weird ABI issues when linking
    target_compile_features(DecemberBliss PRIVATE c_std_11)

endif()

### Included dependencies' source code
add_subdirectory(raylib/)
add_subdirectory(raygui/)

### Declare dependencies
target_link_libraries(DecemberBliss PRIVATE raylib raygui)

### Project source
target_sources(DecemberBliss PRIVATE main.c)



