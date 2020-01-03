//Pulled this one type out of the main raylib.h so it can be included in imgui without
//including the entire raylib.h (which causes compilation issues if you do)

#ifndef __RAYLIB_VEC2__
#define __RAYLIB_VEC2__

// Vector2 type
typedef struct Vector2 {
    float x;
    float y;
} Vector2;

#endif