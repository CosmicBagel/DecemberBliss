#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include "cimgui-master\cimgui.h"
#include "rayIncludes.h"

typedef struct DevUIState {
	bool show_demo_window;
	bool isResourceCounterOpen;
	ImGuiContext* guiContext;
	ImGuiIO* igIO;
} DevUIState;

void DevUIInit(DevUIState* devUIState, void* glfwWindow);
void DevUIDraw(DevUIState* devUIState);
void DevUIRender(void);
void DevUIDestroy(DevUIState* devUIState);