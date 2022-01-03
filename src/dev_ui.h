#pragma once

#include <imgui.h>
#include "../MetricsGui/include/metrics_gui/metrics_gui.h"
#include "rayIncludes.h"

struct Metrics 
{
	MetricsGuiMetric frame_time;
	MetricsGuiMetric input_time;
	MetricsGuiMetric simulation_time;
	MetricsGuiMetric draw_prep_time;
	MetricsGuiMetric drawing_time;
	MetricsGuiMetric dev_ui_time;
	MetricsGuiMetric dev_ui_render_time;
};

class Dev_UI 
{
public:
	Dev_UI();
	~Dev_UI();
	void render();
	void draw();
	void new_frame();

	Metrics metrics;
	bool show_demo_window;
	bool isResourceCounterOpen;
	ImGuiContext *guiContext;
	ImGuiIO *igIO;
	MetricsGuiPlot plot;
	Texture2D fontAtlasTex;

private:
	void draw_resource_counter();
	void init_metrics_gui_plot();
	void init_metrics_gui_metrics();
    Image load_image_from_pixels(unsigned char *pixels, int width, int height, int format);
};
