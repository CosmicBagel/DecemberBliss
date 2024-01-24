
if(MSVC)
    add_compile_options(/W0)
endif()

add_subdirectory(raylib-4.0.0/)
add_subdirectory(imgui-1.85/)
add_subdirectory(MetricsGui/)