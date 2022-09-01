#pragma once

#include "dev_ui.h"
#include "input_state.h"
#include "ray_includes.h"

class Entity;
// struct forward declaration is silly
typedef struct C_Position C_Position;
typedef struct C_Velocity C_Velocity;

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
    static void render_dev_ui();
    static void render_scene();
    void draw_dev_ui();
    void draw_scene();
    void draw_game_ui();
    void simulation_step();
    void handle_input();

    void update_players();
    static void update_velocity();
    void update_enemy_player_collisions();
    void update_enemy_bullet_collisions();
    static void enforce_bullet_boundary();

   private:
    static const int screen_width = 800;
    static const int screen_height = 600;

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
    void create_enemy(size_t count, Vector2 no_spawn_center);
    void create_bullet(C_Position initial_pos, C_Velocity initial_vel,
                       float initial_rot);
};
