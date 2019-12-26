#include <assert.h>
#include <string.h>
#include <raylib/src/external/glad.h>

//-----------opengl

//helper macro stolen from imgui.h
#define IM_OFFSETOF(_TYPE, _MEMBER) ((size_t) & (((_TYPE *)0)->_MEMBER))
#define ImDrawCallback_ResetRenderState (ImDrawCallback)(-1)

#define IM_ALLOC(_SIZE) igMemAlloc(_SIZE);
#define IM_FREE(_PTR) igMemFree(_PTR)

#define IM_ARRAYSIZE(_ARR) ((int)(sizeof(_ARR) / sizeof(*_ARR))) // Size of a static C-style array. Don't use on pointers!
#define IM_UNUSED(_VAR) ((void)_VAR)

#define IM_ASSERT(_EXPR) assert(_EXPR)

// Backend API
bool ImGui_ImplRaylibGL3_Init(const char *glsl_version);
void ImGui_ImplRaylibGL3_Shutdown();
void ImGui_ImplRaylibGL3_NewFrame();
void ImGui_ImplRaylibGL3_RenderDrawData(ImDrawData *draw_data);

// (Optional) Called by Init/NewFrame/Shutdown
bool ImGui_ImplRaylibGL3_CreateFontsTexture();
void ImGui_ImplRaylibGL3_DestroyFontsTexture();
bool ImGui_ImplRaylibGL3_CreateDeviceObjects();
void ImGui_ImplRaylibGL3_DestroyDeviceObjects();

#define IMGUI_IMPL_OPENGL_LOADER_GLAD