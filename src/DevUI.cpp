#include "DevUI.h"
#include "imgui_impl_raylib.h"
#include "rayIncludes.h"

void DrawResourceCounter(DevUIState *devUIState);

void DevUIInit(DevUIState *devUIState)
{
	devUIState->guiContext = ImGui::CreateContext(nullptr);
	devUIState->igIO = &ImGui::GetIO();
	devUIState->show_demo_window = true;
	devUIState->isResourceCounterOpen = false;

    // Initialize imgui
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();

    // ImGui: Initialize keyboard and mouse events
    ImGui_ImplRaylib_Init();

    // ImGui: Init fon
    // ImFont* font = io.Fonts->AddFontDefault();
    unsigned char *pixels = NULL;
    int atlasWidth, atlasHeight;
    io.Fonts->Build();
    io.Fonts->GetTexDataAsRGBA32(&pixels, &atlasWidth, &atlasHeight, NULL);
    // io.Fonts->GetTexDataAsAlpha8(&pixels, &atlasWidth, &atlasHeight, NULL);
    // At this point you've got the texture data and you need to upload that your your graphic system:
    // After we have created the texture, store its pointer/identifier (_in whichever format your engine uses_) in 'io.Fonts->TexID'.
    // This will be passed back to your via the renderer. Basically ImTextureID == void*. Read FAQ for details about ImTextureID.

    Image fontAtlasImage = LoadImageFromPixels(pixels, atlasWidth, atlasHeight,
                                               rlPixelFormat::RL_PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);

    // Image fontAtlasImage = LoadImageFromPixels(pixels, atlasWidth, atlasHeight,
    //     rlPixelFormat::RL_PIXELFORMAT_UNCOMPRESSED_GRAYSCALE);
    devUIState->fontAtlasTex = LoadTextureFromImage(fontAtlasImage);
    // SetTextureFilter(fontAtlasTex, RL_TEXTURE_FILTER_BILINEAR);
    // SetTextureWrap(fontAtlasTex, TEXTURE_WRAP_CLAMP);
    io.Fonts->TexID = (ImTextureID)&devUIState->fontAtlasTex.id;
}

void DevUINewFrame()
{
    ImGui_ImplRaylib_NewFrame();
}

// Updates the DevUI state and prepares all of the draw calls for the DevUI
void DevUIDraw(DevUIState *devUIState)
{
	DrawResourceCounter(devUIState);
}

void DevUIRender()
{
    ImGui::Render();
    raylib_render_imgui(ImGui::GetDrawData());
}

void DevUIDestroy(DevUIState *devUIState)
{
	// DestroyImGui();
    ImGui::DestroyContext(devUIState->guiContext);
    UnloadTexture(devUIState->fontAtlasTex);
}

//Draw the FPS counter window
void DrawResourceCounter(DevUIState *devUIState)
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

    if (devUIState->isResourceCounterOpen)
    {
//        ImGui::SetNextWindowSize({780, 290});
        ImGui::SetNextWindowBgAlpha(1.0f);
    }
    else {
        //sizing is switched to auto resize above
        ImGui::SetNextWindowBgAlpha(0.40f);
    }

	ImGui::Begin("Metrics", nullptr, windowFlags);

    ImGui::TextColored({0.0f, 1.0f, 0.0f, 1.0f}, "FPS: %.2f", devUIState->igIO->Framerate);
    devUIState->isResourceCounterOpen =
            devUIState->isResourceCounterOpen ^ (ImGui::IsWindowHovered(0) & ImGui::IsMouseClicked(0, false) & ImGui::IsItemClicked(0));
	if (devUIState->isResourceCounterOpen)
	{
		ImGui::Separator();
//		ImGui::Text("Frame times (last 120 frames)");
//		ImGui::PlotLines("##", frameTimes, 120, arrayOffset,
//					nullptr, 0.0f, max, {240, 40}, 4);
        devUIState->plot->DrawHistory();
//        devUIState->plot->DrawList();
	}
    ImGui::End();
}

// Load raw pixel data into an image
Image LoadImageFromPixels(unsigned char *pixels, int width, int height, int format)
{
    Image srcImage = {0};

    srcImage.data = pixels;
    srcImage.width = width;
    srcImage.height = height;
    srcImage.mipmaps = 1;
    srcImage.format = format;

    Image dstImage = ImageCopy(srcImage);

    return dstImage;
}