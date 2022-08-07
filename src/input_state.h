#pragma once

#include "ray_includes.h"

struct Input_State
{
public:
    bool exit_window = false;

	bool up;
	bool down;
	bool left;
	bool right;

	Vector2 target_pos;
	bool fire;
	bool alt_fire;
	bool bomb;
};
