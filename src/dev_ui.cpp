#include "dev_ui.h"

#include "imgui_impl_raylib.h"
#include "ray_includes.h"

Dev_UI& Dev_UI::instance() {
    static Dev_UI inst;
    return inst;
}

Dev_UI::Dev_UI()
    : ig_io(nullptr), font_atlas_tex({}), gui_context(nullptr) {}

// need to wait for raylib window to init, which can happen after we construct
// Dev_UI, hence init
void Dev_UI::init() {
    // Initialize imgui
    gui_context = ImGui::CreateContext();
    ig_io = &ImGui::GetIO();

    // ImGui: Initialize keyboard and mouse events
    ImGui_ImplRaylib_Init();

    // ImGui: Init font
    // ImFont* font = io.Fonts->AddFontDefault();
    unsigned char* pixels = NULL;

    int atlas_width = 0;
    int atlas_height = 0;
    ig_io->Fonts->Build();
    ig_io->Fonts->GetTexDataAsRGBA32(&pixels, &atlas_width, &atlas_height,
                                     NULL);
    // At this point you've got the texture data and you need to upload that to
    // your graphic system: After we have created the texture, store its
    // pointer/identifier // (_in whichever format your engine uses_) in
    // 'io.Fonts->TexID'.  This will be passed back to your via the renderer.
    // Basically ImTextureID == void*. Read FAQ for details about ImTextureID.

    IntVector2 atlas_dims = {atlas_width, atlas_height};
    Image fontAtlasImage = Dev_UI::load_image_from_pixels(
        pixels, atlas_dims,
        rlPixelFormat::RL_PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);

    font_atlas_tex = LoadTextureFromImage(fontAtlasImage);
    ig_io->Fonts->TexID = static_cast<ImTextureID>(&font_atlas_tex.id);

    init_metrics_gui_plot();
    init_metrics_gui_metrics();
}

void Dev_UI::new_frame() { ImGui_ImplRaylib_NewFrame(); }

// Updates the DevUI state and prepares all of the draw calls for the DevUI
void Dev_UI::draw() { draw_resource_counter(); }

void Dev_UI::render() {
    ImGui::Render();
    raylib_render_imgui(ImGui::GetDrawData());
}

Dev_UI::~Dev_UI() {
    TraceLog(LOG_INFO, "Unloading Dev_UI...");
    ImGui::DestroyContext(gui_context);
    UnloadTexture(font_atlas_tex);
}

// Draw the FPS counter window
void Dev_UI::draw_resource_counter() {
    const ImVec2 fps_draw_pos = {10, 10};
    const float fps_alpha = 0.40F;
    ImGui::SetNextWindowPos(fps_draw_pos, ImGuiCond_Appearing, {0, 0});
    ImGui::SetNextWindowBgAlpha(fps_alpha);

    ImGuiWindowFlags windowFlags =  // ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoNav | ImGuiWindowFlags_AlwaysAutoResize |
        ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize;

    if (is_resource_counter_open) {
        //        ImGui::SetNextWindowSize({780, 290});
        ImGui::SetNextWindowBgAlpha(1.0F);
    } else {
        // sizing is switched to auto resize above
        ImGui::SetNextWindowBgAlpha(fps_alpha);
    }

    ImGui::Begin("Metrics", nullptr, windowFlags);

    ImGui::TextColored({0.0F, 1.0F, 0.0F, 1.0F}, "FPS: %.2F", ig_io->Framerate);

    bool is_clicked = ImGui::IsWindowHovered(0) &&
                      ImGui::IsMouseClicked(0, false) &&
                      ImGui::IsItemClicked(0);
    if (is_clicked) {
        is_resource_counter_open = !is_resource_counter_open;
    }

    if (is_resource_counter_open) {
        ImGui::Separator();
        //		ImGui::Text("Frame times (last 120 frames)");
        //		ImGui::PlotLines("##", frameTimes, 120, arrayOffset,
        //					nullptr, 0.0f, max, {240, 40}, 4);
        plot.DrawHistory();
        //        devUIState->plot->DrawList();
    }
    ImGui::End();
}

void Dev_UI::init_metrics_gui_plot() {
    // this whole function is literally just assigning numbers to named values
    // declaring them before hand as constants would not make this more readable
    // NOLINTBEGIN(*-magic-numbers)
    plot.mMinPlotHeight = 250.0F;
    plot.mMinPlotWidth = 535.0F;
    plot.mBarRounding = 0.0F;  // amount of rounding on bars
    plot.mRangeDampening =
        0.95F;  // weight of historic range on axis range [0,1]
    plot.mInlinePlotRowCount =
        2;                   // height of DrawList() inline plots, in text rows
    plot.mPlotRowCount = 5;  // height of DrawHistory() plots, in text rows
    plot.mVBarMinWidth = 1;  // min width of bar graph bar in pixels
    plot.mVBarGapWidth = 0;  // width of bar graph inter-bar gap in pixels
    plot.mShowAverage = false;         // draw horizontal line at series average
    plot.mShowInlineGraphs = true;     // show history plot in DrawList()
    plot.mShowOnlyIfSelected = false;  // draw show selected metrics
    plot.mShowLegendDesc = true;       // show series description in legend
    plot.mShowLegendColor = true;      // use series colour in legend
    plot.mShowLegendUnits = true;      // show units in legend values
    plot.mShowLegendAverage = true;    // show series average in legend
    plot.mShowLegendMin = true;        // show plot y-axis minimum in legend
    plot.mShowLegendMax = true;        // show plot y-axis maximum in legend
    plot.mBarGraph = true;             // use bars to draw history
    plot.mStacked = true;              // stack series when drawing history
    plot.mSharedAxis = false;          // use first series' axis range
    plot.mFilterHistory = true;  // allow single plot point to represent more
                                 // than on history value
    // NOLINTEND(*-magic-numbers)
}

void Dev_UI::init_metrics_gui_metrics() {
    TraceLog(LOG_INFO, "Initializing metric trackers");

    auto const FLAG = MetricsGuiMetric::USE_SI_UNIT_PREFIX;
    const auto* const UNITS = "us";

    auto& metrics = Metrics::instance();

    metrics.frame_time.mDescription = "Frame Time";
    metrics.frame_time.mUnits = UNITS;
    metrics.frame_time.mFlags = FLAG;

    metrics.input_time.mDescription = "Input Handling Time";
    metrics.input_time.mUnits = UNITS;
    metrics.input_time.mFlags = FLAG;

    metrics.simulation_time.mDescription = "Simulation";
    metrics.simulation_time.mUnits = UNITS;
    metrics.simulation_time.mFlags = FLAG;

    metrics.draw_prep_time.mDescription = "Draw Prep";
    metrics.draw_prep_time.mUnits = UNITS;
    metrics.draw_prep_time.mFlags = FLAG;

    metrics.game_ui_time.mDescription = "Draw Prep UI";
    metrics.game_ui_time.mUnits = UNITS;
    metrics.game_ui_time.mFlags = FLAG;

    metrics.drawing_time.mDescription = "RL EndDrawing";
    metrics.drawing_time.mUnits = UNITS;
    metrics.drawing_time.mFlags = FLAG;

    metrics.dev_ui_time.mDescription = "Dev UI";
    metrics.dev_ui_time.mUnits = UNITS;
    metrics.dev_ui_time.mFlags = FLAG;

    metrics.dev_ui_render_time.mDescription = "Dev UI Render";
    metrics.dev_ui_render_time.mUnits = UNITS;
    metrics.dev_ui_render_time.mFlags = FLAG;

    metrics.ecs_bookkeeping_time.mDescription = "ECS Bookkeeping";
    metrics.ecs_bookkeeping_time.mUnits = UNITS;
    metrics.ecs_bookkeeping_time.mFlags = FLAG;

    // plot.AddMetric(&metrics.frame_time);
    plot.AddMetric(&metrics.input_time);
    plot.AddMetric(&metrics.simulation_time);
    plot.AddMetric(&metrics.draw_prep_time);
    plot.AddMetric(&metrics.game_ui_time);
    plot.AddMetric(&metrics.drawing_time);
    plot.AddMetric(&metrics.dev_ui_time);
    plot.AddMetric(&metrics.dev_ui_render_time);
    plot.AddMetric(&metrics.ecs_bookkeeping_time);
}

// Load raw pixel data into an image
Image Dev_UI::load_image_from_pixels(unsigned char* pixels,
                                     IntVector2 dimensions, int format) {
    Image srcImage = {0};

    srcImage.data = pixels;
    srcImage.width = dimensions.x;
    srcImage.height = dimensions.y;
    srcImage.mipmaps = 1;
    srcImage.format = format;

    Image dstImage = ImageCopy(srcImage);

    return dstImage;
}