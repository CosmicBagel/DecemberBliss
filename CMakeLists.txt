cmake_minimum_required(VERSION 3.0.0)
project(DecemberBliss VERSION 0.1.0)

include(BlissBuildHelpers.cmake)

# Trying to stick with Effective Modern CMake https://gist.github.com/mbinna/c61dbb39bca0e4fb7d1f73b0d66a4fd1

# use message(STATUS "message info ${variable}") for rnd print statements

#lists are ; separated stings
#CMake variables are NOT environment variables (unlike makefile)

#avoid custom variables in arguments of project commands

#[==[
 arbitrary = count in comment start, must match the number in comment end
 multiline comment block
   #[=[
    some kind of nesting functionality??
   #]=]
#]==]

#[[
don't use variables, but 
   set(varName varText) varName and varText are strings, and don't need ""
   unset(varName) makes a string empty
#]]

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

#18:33

#[[
    A generator expression
    $<IF:$<CONFIG:Debug>,30,10>
    $<> syntax
    not evaluated by command interpreter (it is just a string with $<>)
    evaluated during build system generation
    not supported in all commands
 #]]

#if(thing) endif()

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


# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:main")

# all cmake properties https://cmake.org/cmake/help/latest/manual/cmake-properties.7.html#target-properties
# available c compile features https://cmake.org/cmake/help/latest/prop_gbl/CMAKE_C_KNOWN_FEATURES.html#prop_gbl:CMAKE_C_KNOWN_FEATURES 
# avialable c++ compile features https://cmake.org/cmake/help/latest/prop_gbl/CMAKE_CXX_KNOWN_FEATURES.html#prop_gbl:CMAKE_CXX_KNOWN_FEATURES

# avoid custom variables in the arguments of project commands
# do not use file(GLOB) in project
# Don't abouse requirements, Eg -Wall is not a requirement to build a project

#[[
todo
- Project name & version injected into c code
- include raylib as subdirectory?
    - figure out how to source and link library
#]]

add_executable(DecemberBliss)
target_compile_features(DecemberBliss PRIVATE c_std_11)
bliss_enable_warnings(DecemberBliss)

target_include_directories(DecemberBliss PRIVATE raygui)

add_subdirectory(raylib)
target_link_libraries(DecemberBliss PRIVATE raylib)

# set_target_properties(DecemberBliss PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")

target_sources(DecemberBliss PRIVATE main.c)



