#pragma once

#include "ray_includes.h"
#include "dev_ui.h"
#include "input_state.h"

class Bliss_App
{
public:
	Bliss_App();
    ~Bliss_App();
	void run();
    void render_dev_ui();
    void render_scene();
    void draw_dev_ui();
    void draw_scene();
    void draw_game_ui();
    void simulation_step();
    void handle_input();
private:
    int screen_width  = 800;
    int screen_height = 600;

    const int STARTING_LIVES = 3;
    int lives = STARTING_LIVES;

    Dev_UI& dev_ui;

    void load_fonts();
    Font font_roboto_mono;
    Font font_roboto_mono_sm;
    Font font;

    void load_textures();
    Texture2D santa_tex;
    Texture2D santa_cropped_tex;
    Texture2D santa_sm_tex;
    Texture2D popper_sm_tex;
    Texture2D snowball_tex;

    Input_State input_state;

    void create_player();
    void create_enemy(int count = 7);
    void create_bullet(float pos_x, float pos_y, float vel_x, float vel_y, float rot);
};




