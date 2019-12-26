#include "DevUI.h"

#include "imgui_impl_raylib.h"
#include "imgui_impl_raylibgl3.h"

void DrawResourceCounter(DevUIState* devUIState);

void DevUIInit(DevUIState* devUIState, void* glfwWindow)
{
	devUIState->guiContext = igCreateContext(NULL);
	devUIState->igIO = igGetIO();
	devUIState->show_demo_window = true;
	devUIState->isResourceCounterOpen = false;

	igStyleColorsDark(NULL);
	ImGui_ImplRaylib_InitForOpenGL(glfwWindow, true);
	const char* glsl_version = "#version 130";
	ImGui_ImplRaylibGL3_Init(glsl_version);
}

void DevUINewFrame()
{
	ImGui_ImplRaylibGL3_NewFrame();
	ImGui_ImplRaylib_NewFrame();
	igNewFrame();
}

// Updates the DevUI state and prepares all of the draw calls for the DevUI
void DevUIDraw(DevUIState* devUIState)
{
	DrawResourceCounter(devUIState);
	igShowDemoWindow(&devUIState->show_demo_window);
}

void DevUIRender(void)
{
	igRender();
	ImGui_ImplRaylibGL3_RenderDrawData(igGetDrawData());
}

void DevUIDestroy(DevUIState* devUIState)
{
	ImGui_ImplRaylibGL3_Shutdown();
	ImGui_ImplRaylib_Shutdown();
	igDestroyContext(devUIState->guiContext);
}

//Draw the FPS counter window
void DrawResourceCounter(DevUIState* devUIState)
{
	// todo
	// - use PushID popID to customize style of this window
	// - add graphs as hideable children
	// - make moveable with right click
	// - add show/hide hotkey

	ImGuiWindowFlags windowFlags = 0 |
		ImGuiWindowFlags_NoDecoration |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoNav | 
		ImGuiWindowFlags_AlwaysAutoResize | 
		ImGuiWindowFlags_NoSavedSettings |
		//ImGuiWindowFlags_Modal |
		ImGuiWindowFlags_NoResize;

	igSetNextWindowPos((ImVec2) { 10, 10 }, ImGuiCond_Appearing, (ImVec2) { 0, 0 });
	igSetNextWindowBgAlpha(0.40f);
	//igSetNextWindowFocus();

	char fpsStr[50] = { '\0' };
	sprintf_s(fpsStr, 50, "FPS: %.2f", devUIState->igIO->Framerate);
	static float frameTimes[120] = {0};
	static unsigned int arrayOffset = 0;
	frameTimes[arrayOffset] = GetFrameTime() * 1000.0f;
	arrayOffset = (arrayOffset + 1) % 120;

	float max = 0.0f;
	for (int i = 0; i < 120; i++)
		if (max < frameTimes[i])
			max = frameTimes[i];
	
	igBegin("Resource Counter", 0, windowFlags);
	devUIState->isResourceCounterOpen =
		devUIState->isResourceCounterOpen ^ (igIsWindowHovered(0) & igIsMouseClicked(0, 0));
	igTextColored((ImVec4) { 0.0f, 1.0f, 0.0f, 1.0f }, fpsStr);
	if (devUIState->isResourceCounterOpen)
	{
		igSeparator();
		igText("Frame times (last 120 frames)");
		igPlotLines("##", frameTimes, 120, arrayOffset,
			NULL, 0.0f, max, (ImVec2) { 240, 40 }, 4);
	}
	igEnd();
}