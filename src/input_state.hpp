#pragma once

#include "ray_includes.hpp"

struct Input_State {
   public:
    bool exit_window = false;

    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;

    Vector2 target_pos {0.0F, 0.0F};
    bool fire = false;
    bool alt_fire = false;
    bool bomb = false;
};
