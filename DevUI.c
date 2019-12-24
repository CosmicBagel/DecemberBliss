#include "DevUI.h"

#include "imgui_impl_raylib.h"
#include "imgui_impl_raylibgl3.h"

void DrawResourceCounter(DevUIState* devUIState);

void DevUIInit(DevUIState* devUIState, void* glfwWindow)
{
	devUIState->guiContext = igCreateContext(NULL);
	devUIState->igIO = igGetIO();
	devUIState->show_demo_window = true;

	igStyleColorsDark(NULL);
	ImGui_ImplRaylib_InitForOpenGL(glfwWindow, true);
	const char* glsl_version = "#version 130";
	ImGui_ImplRaylibGL3_Init(glsl_version);
}

// Updates the DevUI state and prepares all of the draw calls for the DevUI
void DevUIDraw(DevUIState* devUIState)
{
	igSetCurrentContext(devUIState->guiContext);

	ImGui_ImplRaylibGL3_NewFrame();
	ImGui_ImplRaylib_NewFrame();
	igNewFrame();

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
	// - add graphs as hideable children
	// - make moveable with right click
	// - add show/hide hotkey

	ImGuiWindowFlags fpsTrackerWindowFlags = 0 |
		//ImGuiWindowFlags_NoInputs |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize;
		//ImGuiWindowFlags_NoTitleBar;

	if (devUIState->isResourceCounterOpen)
	{
		igSetNextWindowSize((ImVec2) { 256, 256 }, ImGuiCond_Always);
	}
	else
	{
		igSetNextWindowSize((ImVec2) { 120, 256 }, ImGuiCond_Always);
	}

	igSetNextWindowPos((ImVec2) { 10, 10 }, ImGuiCond_Appearing, (ImVec2) { 0, 0 });
	igSetNextWindowBgAlpha(0.40f);

	char fpsStr[50] = { '\0' };
	sprintf_s(fpsStr, 50, "FPS: %.1f###0", devUIState->igIO->Framerate);
	static float frameTimes[120] = {0};
	static unsigned int arrayOffset = 0;
	frameTimes[arrayOffset] = 1 / devUIState->igIO->Framerate * 1000.0f;
	arrayOffset = (arrayOffset + 1) % 120;

	float max = 0.0f;
	for (int i = 0; i < 120; i++)
		if (max < frameTimes[i])
			max = frameTimes[i];
	
	igBegin(fpsStr, 0, fpsTrackerWindowFlags);
	devUIState->isResourceCounterOpen = !igIsWindowCollapsed();
	igPlotHistogramFloatPtr("", frameTimes, 120, arrayOffset, 
		NULL, 0.0f, max, (ImVec2) { 0, 40 }, 4);
	igTextColored((ImVec4) { 0.0f, 5.0f, 0.0f, 1.0f }, "cool metrics");




	igEnd();
}