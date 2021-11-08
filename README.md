# DecemberBliss

A learning project to get more familiar with using c and c++ in a more practical situation.

The code is very messy as I'm prototyping a lot of things I'm not familiar with yet, once I've established how to 
make these systems talk to each other in a way I like, I'll be rewriting a significant chunk of the code with a proper
design in mind. I'm just muddling forward trying to figure out what works currently.

The end goal is to have a fairly barebones 2d game engine functioning, complete with:
- "Reasonable" performance (144fps with minimal frame stuttering, targeting a low end integrated graphics laptop)
- Very simple dev tools, built using dear imgui
- Simple 2d physics

Building requires
- A c/c++ compiler, complete with required std libs for the platform you're building on
- Python (3.8 or newer) installation matching the target architecture (x86 or x64)
    - Be sure to select "Download debug binaries" if you want to build a debug build
- CMake
    - You must pass in the variable BLISS_ARCH as x86 or x64
- Ninja (not required, but its sure does speed things up!)

I tried to include as many dependencies as possible so as to simplify the build process

State of the project as of Nov 7th, 2021
- Resurrecting this project
- Removed chipmunk and entt (out of scope for now)
- Keeping python bits, but I have a specific idea of how I want python to interact with the
  game console that will take a long time to work out, effectively python is on hold until 
  further notice
- Updated Raylib and Imgui (this was a lot more pain than it was worth, mostly due to raylib's
  very unstable api)
- New imgui/raylib backend (a modified backend of https://github.com/WEREMSOFT/c99-raylib-cimgui-template 
  for c++ and raylib 4.0; I used this base as it was working with a more recent version of raylib so
  there was less pain trying to figure out how raysan mixed up the API)

State of the project as of Jan 13th, 2020
- Python is in, but there is no scripting functionality or in-game console to leverage it with yet
    - Working on a solution to ensure print output is channeled to the in-game log/console (currently the log & console are not separated)
- Some basic metrics are tracked, visible when you click the FPS counter in the top left

Screenshot
![screenshot](https://i.imgur.com/JXfpEXL.png)

My VS Code Cmake workspace settings:
```
"cmake.generator": "Ninja",
"cmake.configureSettings": {
    "BLISS_ARCH": "x64",
    "CMAKE_BUILD_TYPE": "${buildType}"
},
```