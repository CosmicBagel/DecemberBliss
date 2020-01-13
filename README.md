# DecemberBliss

A learning project to get more familiar with using c and c++ in a more practical situation.

The code is very messy as I'm prototyping a lot of things I'm not familiar with yet, once I've established how to 
make these systems talk to each other in a way I like, I'll be rewriting a significant chunk of the code with a proper
design in mind. I'm just muddling forward trying to figure out what works currently.

The end goal is to have a fairly barebones 2d game engine functioning, complete with:
- Python scripting
- 2d physics
- "Reasonable" performance (60fps with minimal frame stuttering, targeting a low end integrated graphics laptop)
- Very simple dev tools, built using dear imgui

Building requries
- A c/c++ compiler, complete with requried std libs for the platform you're building on
- Python (3.8 or newer) installation matching the target architecture (x86 or x64)
- CMake

I tried to include as many dependencies as possible so as to simplify the build process

State of the project as of Jan 13th, 2020
- Python is in, but there is no scritping functionality or in-game console to leverage it with yet
    - Working on a solution to ensure print output is channeled to the in-game log/console (currently the log & console are not separated)
- Some basic metrics are tracked, visble when you click the FPS coutner in the top left
