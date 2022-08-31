#pragma once

#include <../MetricsGui/include/metrics_gui/metrics_gui.h>
#include <imgui.h>

#include "ray_includes.h"

struct Metrics {
    MetricsGuiMetric frame_time;
    MetricsGuiMetric input_time;
    MetricsGuiMetric simulation_time;
    MetricsGuiMetric draw_prep_time;
    MetricsGuiMetric drawing_time;
    MetricsGuiMetric dev_ui_time;
    MetricsGuiMetric game_ui_time;
    MetricsGuiMetric dev_ui_render_time;
    MetricsGuiMetric ecs_bookkeeping_time;
};

class IntVector2 {
   public:
    int x;
    int y;
};

class Dev_UI {
   public:
    static Dev_UI& instance();

    // no copy ctor, move ctor, copy assn, move assn
    // since this class is meant to be the program
    // when this class destructs, the program ends
    Dev_UI(const Dev_UI&) = delete;
    Dev_UI(Dev_UI&&) = delete;
    Dev_UI& operator=(Dev_UI) = delete;
    Dev_UI& operator=(Dev_UI&&) = delete;

    void init();
    static void render();
    void draw();
    static void new_frame();

    void update_plot_axis() { plot.UpdateAxes();}

    Metrics metrics;

   private:
    // Dev_UI();
    ~Dev_UI();
    void draw_resource_counter();
    void init_metrics_gui_plot();
    void init_metrics_gui_metrics();
    static Image load_image_from_pixels(unsigned char* pixels,
                                        IntVector2 dimensions, int format);

    bool show_demo_window = true;
    bool is_resource_counter_open = false;

    ImGuiIO* ig_io;
    Texture2D font_atlas_tex;
    ImGuiContext* gui_context;
    MetricsGuiPlot plot;
};
