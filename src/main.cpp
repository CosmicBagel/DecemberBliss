/*
TODO: Bring back the batched dev ui rendering, the new one is too slow
TODO: Implement instrumented profiling (see visual code profiling notes)
TODO: Implement ECS with memory pooling and nice cache locality properties
TODO: Clean up main.cpp (semantic compression)
TODO: Setup precompiled headers as imgui, raylib, and metrics gui are never \
going to change
TODO: Switch to consistent naming convention everywhere (Title_Case for types \
snake_case for vars and funcs, SCREAMING_SNAKE_CASE for constants) Title_Case \
for files
TODO: Memory usage tracking
*/

#include "bliss_app.h"

int main()
{
    Bliss_App app;
    app.app_run();
}
