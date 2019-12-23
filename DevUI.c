#include "DevUI.h"

#include "imgui_impl_raylib.h"
#include "imgui_impl_raylibgl3.h"

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

	DrawResourceCounter(devUIState->igIO);
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
void DrawResourceCounter(ImGuiIO* igIO)
{
	ImGuiWindowFlags fpsTrackerWindowFlags = 0 |
		ImGuiWindowFlags_NoInputs |
		ImGuiWindowFlags_NoTitleBar;
	// todo
	// - add graphs as hideable children
	// - make moveable with right click
	// - add show/hide hotkey

	//ImGuiStyle style = ;

	igSetNextWindowPos((ImVec2) { 10, 10 }, ImGuiCond_Once, (ImVec2) { 0, 0 });
	igSetNextWindowSize((ImVec2) { 90, 28 }, ImGuiCond_Once);
	igSetNextWindowBgAlpha(0.40f);
	igBegin("Resource Counter", 0, fpsTrackerWindowFlags);
	char fpsStr[50] = { '\0' };
	sprintf_s(fpsStr, 50, "FPS: %.1f", igIO->Framerate);
	igTextColored((ImVec4) { 0.0f, 5.0f, 0.0f, 1.0f }, fpsStr);
	igEnd();
}