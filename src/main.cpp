/*
TODO: Bring back the batched dev ui rendering, the new one is too slow
TODO: Implement instrumented profiling (see visual code profiling notes)
TODO: Implement ECS with memory pooling and nice cache locality properties
TODO: Setup precompiled headers as imgui, raylib, and metrics gui are never \
going to change
TODO: Switch to consistent naming convention everywhere (Title_Case for types \
snake_case for vars and funcs, SCREAMING_SNAKE_CASE for constants) Title_Case \
for files
TODO: Memory usage tracking
TODO: cross compiler compatible strcpy_s (use c11?
https://en.cppreference.com/w/c/string/byte/strcpy)
TODO: redesign ECS without tuple (tuple is just too slow)
*/

/*
TODO death state with "you died press P to reset" message
TODO P to reset
TODO enemies move in some semi-random way (sinusoidal patterns)
TODO player death wipes screen with a circle radiating outward
TODO player has 3 lives (P does a hard reset)
TODO points?
TODO spawning system
TODO alt fire clears the screen with fast moving circle
TODO controller support
*/

#include "bliss_app.hpp"

int main() {
    Bliss_App app;
    app.run();
}
