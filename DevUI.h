#include <imgui.h>
#include "MetricsGui/include/metrics_gui/metrics_gui.h"

struct DevUIState
{
	bool show_demo_window;
	bool isResourceCounterOpen;
	ImGuiContext *guiContext;
	ImGuiIO *igIO;
	MetricsGuiPlot *plot;
};

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

void DevUIInit(DevUIState *devUIState);
void DevUINewFrame();
void DevUIDraw(DevUIState *devUIState);
void DevUIRender();
void DevUIDestroy(DevUIState *devUIState);