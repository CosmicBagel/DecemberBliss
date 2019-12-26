#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include "cimgui/cimgui.h"
#include "rayIncludes.h"

typedef struct DevUIState
{
	bool show_demo_window;
	bool isResourceCounterOpen;
	ImGuiContext *guiContext;
	ImGuiIO *igIO;
} DevUIState;

typedef struct ResourceMetrics
{
	float lastFrameSimulationTime;
	float lastFrameDrawPrepTime;
	float lastFrameGPURenderTime;

	float gameDrawPrepTime;
	float gameGPURenderTime;
	float devUIDrawPrepTime;
	float devUIGPURenderTime;

	unsigned int memoryUsage;
	unsigned int gameMemoryUsage;
	unsigned int devUIMemoryUsage;

} ResourceMetrics;

void DevUIInit(DevUIState *devUIState, void *glfwWindow);
void DevUINewFrame(void);
void DevUIDraw(DevUIState *devUIState);
void DevUIRender(void);
void DevUIDestroy(DevUIState *devUIState);