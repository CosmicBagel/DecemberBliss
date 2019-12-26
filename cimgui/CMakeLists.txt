cmake_minimum_required(VERSION 3.1)

#add library and link
add_library(cimgui STATIC cimgui.cpp)

target_compile_definitions(cimgui PUBLIC IMGUI_DISABLE_OBSOLETE_FUNCTIONS=1)
if (WIN32)
    target_compile_definitions(cimgui PUBLIC IMGUI_IMPL_API="extern \"C\" __declspec\(dllexport\)")
else (WIN32)
    target_compile_definitions(cimgui PUBLIC IMGUI_IMPL_API="extern \"C\" ")
endif (WIN32)

target_include_directories(cimgui INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(cimgui PRIVATE imgui)
# set_target_properties(cimgui PROPERTIES PREFIX "")

# #install
# install(TARGETS cimgui
#     RUNTIME DESTINATION  .
#     LIBRARY DESTINATION  .
#     ARCHIVE DESTINATION  .
# )

# #test
# set(CIMGUI_TEST "no" CACHE STRING "Enable compilation of a test unit based on imgui null")

# if (CIMGUI_TEST)
#   add_subdirectory(test)
# endif ()
