#include "dev_ui.h"
#include "imgui_impl_raylib.h"
#include "ray_includes.h"

Dev_UI::Dev_UI()
{
    guiContext = ImGui::CreateContext(nullptr);
    igIO = &ImGui::GetIO();
    show_demo_window = true;
    isResourceCounterOpen = false;

    // Initialize imgui
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    // ImGui: Initialize keyboard and mouse events
    ImGui_ImplRaylib_Init();

    // ImGui: Init font
    // ImFont* font = io.Fonts->AddFontDefault();
    unsigned char* pixels = NULL;
    int atlasWidth, atlasHeight;
    io.Fonts->Build();
    io.Fonts->GetTexDataAsRGBA32(&pixels, &atlasWidth, &atlasHeight, NULL);
    // io.Fonts->GetTexDataAsAlpha8(&pixels, &atlasWidth, &atlasHeight, NULL);
    // At this point you've got the texture data and you need to upload that to your graphic system:
    // After we have created the texture, store its pointer/identifier (_in whichever format your engine uses_) in 'io.Fonts->TexID'.
    // This will be passed back to your via the renderer. Basically ImTextureID == void*. Read FAQ for details about ImTextureID.

    Image fontAtlasImage = load_image_from_pixels(
        pixels, atlasWidth, atlasHeight,
        rlPixelFormat::RL_PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);

    // Image fontAtlasImage = LoadImageFromPixels(pixels, atlasWidth, atlasHeight,
    //     rlPixelFormat::RL_PIXELFORMAT_UNCOMPRESSED_GRAYSCALE);
    fontAtlasTex = LoadTextureFromImage(fontAtlasImage);
    // SetTextureFilter(fontAtlasTex, RL_TEXTURE_FILTER_BILINEAR);
    // SetTextureWrap(fontAtlasTex, TEXTURE_WRAP_CLAMP);
    io.Fonts->TexID = (ImTextureID)&fontAtlasTex.id;

    init_metrics_gui_plot();
    init_metrics_gui_metrics();
}

void Dev_UI::new_frame()
{
    ImGui_ImplRaylib_NewFrame();
}

// Updates the DevUI state and prepares all of the draw calls for the DevUI
void Dev_UI::draw()
{
    draw_resource_counter();
}

void Dev_UI::render()
{
    ImGui::Render();
    raylib_render_imgui(ImGui::GetDrawData());
}

Dev_UI::~Dev_UI()
{
    ImGui::DestroyContext(guiContext);
    UnloadTexture(fontAtlasTex);
}

//Draw the FPS counter window
void Dev_UI::draw_resource_counter()
{
    ImGui::SetNextWindowPos({ 10, 10 }, ImGuiCond_Appearing, { 0, 0 });
    ImGui::SetNextWindowBgAlpha(0.40f);

    ImGuiWindowFlags windowFlags = //ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoNav |
        ImGuiWindowFlags_AlwaysAutoResize |
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoResize;

    if (isResourceCounterOpen)
    {
        //        ImGui::SetNextWindowSize({780, 290});
        ImGui::SetNextWindowBgAlpha(1.0f);
    }
    else {
        //sizing is switched to auto resize above
        ImGui::SetNextWindowBgAlpha(0.40f);
    }

    ImGui::Begin("Metrics", nullptr, windowFlags);

    ImGui::TextColored({ 0.0f, 1.0f, 0.0f, 1.0f }, "FPS: %.2f", igIO->Framerate);
    isResourceCounterOpen =
        isResourceCounterOpen ^ (ImGui::IsWindowHovered(0) & ImGui::IsMouseClicked(0, false) & ImGui::IsItemClicked(0));
    if (isResourceCounterOpen)
    {
        ImGui::Separator();
        //		ImGui::Text("Frame times (last 120 frames)");
        //		ImGui::PlotLines("##", frameTimes, 120, arrayOffset,
        //					nullptr, 0.0f, max, {240, 40}, 4);
        plot.DrawHistory();
        //        devUIState->plot->DrawList();
    }
    ImGui::End();
}

void Dev_UI::init_metrics_gui_plot()
{
    plot.mMinPlotHeight = 250.0f;
    plot.mMinPlotWidth = 535.0f;
    plot.mBarRounding = 0.f;          // amount of rounding on bars
    plot.mRangeDampening = 0.95f;     // weight of historic range on axis range [0,1]
    plot.mInlinePlotRowCount = 2;     // height of DrawList() inline plots, in text rows
    plot.mPlotRowCount = 5;           // height of DrawHistory() plots, in text rows
    plot.mVBarMinWidth = 1;           // min width of bar graph bar in pixels
    plot.mVBarGapWidth = 0;           // width of bar graph inter-bar gap in pixels
    plot.mShowAverage = false;        // draw horizontal line at series average
    plot.mShowInlineGraphs = true;    // show history plot in DrawList()
    plot.mShowOnlyIfSelected = false; // draw show selected metrics
    plot.mShowLegendDesc = true;      // show series description in legend
    plot.mShowLegendColor = true;     // use series colour in legend
    plot.mShowLegendUnits = true;     // show units in legend values
    plot.mShowLegendAverage = true;   // show series average in legend
    plot.mShowLegendMin = true;       // show plot y-axis minimum in legend
    plot.mShowLegendMax = true;       // show plot y-axis maximum in legend
    plot.mBarGraph = true;            // use bars to draw history
    plot.mStacked = true;             // stack series when drawing history
    plot.mSharedAxis = false;         // use first series' axis range
    plot.mFilterHistory = true;       // allow single plot point to represent more than on history value
}

void Dev_UI::init_metrics_gui_metrics()
{
    TraceLog(LOG_INFO, "Initializing metric trackers");

    auto const FLAG = MetricsGuiMetric::USE_SI_UNIT_PREFIX;
    auto const UNITS = "us";

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

    metrics.drawing_time.mDescription = "RL EndDrawing";
    metrics.drawing_time.mUnits = UNITS;
    metrics.drawing_time.mFlags = FLAG;

    metrics.dev_ui_time.mDescription = "Dev UI";
    metrics.dev_ui_time.mUnits = UNITS;
    metrics.dev_ui_time.mFlags = FLAG;

    metrics.dev_ui_render_time.mDescription = "Dev UI Render";
    metrics.dev_ui_render_time.mUnits = UNITS;
    metrics.dev_ui_render_time.mFlags = FLAG;

    //plot.AddMetric(&metrics.frame_time);
    plot.AddMetric(&metrics.input_time);
    plot.AddMetric(&metrics.simulation_time);
    plot.AddMetric(&metrics.draw_prep_time);
    plot.AddMetric(&metrics.drawing_time);
    plot.AddMetric(&metrics.dev_ui_time);
    plot.AddMetric(&metrics.dev_ui_render_time);
}

// Load raw pixel data into an image
Image Dev_UI::load_image_from_pixels(unsigned char* pixels, int width, int height, int format)
{
    Image srcImage = { 0 };

    srcImage.data = pixels;
    srcImage.width = width;
    srcImage.height = height;
    srcImage.mipmaps = 1;
    srcImage.format = format;

    Image dstImage = ImageCopy(srcImage);

    return dstImage;
}