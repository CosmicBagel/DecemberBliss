#pragma once

#include "dev_ui.h"
#include "input_state.h"
#include "ray_includes.h"

class Entity;

class Bliss_App {
   public:
    Bliss_App();
    ~Bliss_App();

    // no copy ctor, move ctor, copy assn, move assn
    // since this class is meant to be the program
    // when this class destructs, the program ends
    Bliss_App(const Bliss_App&) = delete;
    Bliss_App(Bliss_App&&) = delete;
    Bliss_App& operator=(Bliss_App) = delete;
    Bliss_App& operator=(Bliss_App&&) = delete;

    void run();
    void render_dev_ui();
    void render_scene();
    void draw_dev_ui();
    void draw_scene();
    void draw_game_ui();
    void simulation_step();
    void handle_input();

   private:
    static const int screen_width = 800;
    static const int screen_height = 600;
    static const float no_spawn_radius;

    static const int STARTING_LIVES = 3;
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

    Entity create_player();
    void create_enemy(int count, Vector2 no_spawn_center,
                      float no_spawn_radius);
    void create_bullet(float pos_x, float pos_y, float vel_x, float vel_y,
                       float rot);
};
