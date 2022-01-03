#pragma once

#include "ray_includes.h"
#include "dev_ui.h"

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
    void simulation_step();
    void handle_input();
private:
    bool exit_window = false;
    int screen_width = 800;
    int screen_height = 600;

    Dev_UI dev_ui;

    void load_fonts();
    Font font_roboto_mono;
    Font font_roboto_mono_sm;
    Font font;

    void load_textures();
    Texture2D santa_tex;

};




