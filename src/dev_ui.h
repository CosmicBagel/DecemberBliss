#pragma once

#include <imgui.h>
#include <../MetricsGui/include/metrics_gui/metrics_gui.h>
#include "ray_includes.h"

struct Metrics
{
    MetricsGuiMetric frame_time;
    MetricsGuiMetric input_time;
    MetricsGuiMetric simulation_time;
    MetricsGuiMetric draw_prep_time;
    MetricsGuiMetric drawing_time;
    MetricsGuiMetric dev_ui_time;
    MetricsGuiMetric dev_ui_render_time;
    MetricsGuiMetric ecs_bookkeeping_time;
};

class Dev_UI
{
public:
    static Dev_UI& instance();
    void init();
    void render();
    void draw();
    void new_frame();

    Metrics        metrics;
    ImGuiIO*       ig_io;
    MetricsGuiPlot plot;

private:
    Dev_UI();
    ~Dev_UI();
    void draw_resource_counter();
    void init_metrics_gui_plot();
    void init_metrics_gui_metrics();
    Image load_image_from_pixels(unsigned char* pixels, int width, int height, int format);

    bool show_demo_window         = true;
    bool is_resource_counter_open = false;

    Texture2D     font_atlas_tex;
    ImGuiContext* gui_context;
};
