#include <stdio.h>
#include <stdint.h>
#include <float.h>
#include <raylib.h>
#include <rlgl.h>
#include "raylib/src/external/glad.h"

#include <glfw/glfw3.h>
#include <GLFW/glfw3native.h>

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <cimgui.h>

#include "imgui_impl_raylib.h"

//ImGui_ImplRaylib_InitForOpenGL(window, true);
const char *glsl_version = "#version 130";
//ImGui_ImplOpenGL3_Init(glsl_version);

/*
	custom imgui initialization layer implementation for raylib

	if possible, remove the GLFW window exposing function from raylib that was added

*/

// OpenGL Data
static GLuint g_GlVersion = 0;			  // Extracted at runtime using GL_MAJOR_VERSION, GL_MINOR_VERSION queries.
static char g_GlslVersionString[32] = ""; // Specified by user or detected based on compile time GL settings.
static GLuint g_FontTexture = 0;
static GLuint g_ShaderHandle = 0, g_VertHandle = 0, g_FragHandle = 0;
static int g_AttribLocationTex = 0, g_AttribLocationProjMtx = 0;								// Uniforms location
static int g_AttribLocationVtxPos = 0, g_AttribLocationVtxUV = 0, g_AttribLocationVtxColor = 0; // Vertex attributes location
static unsigned int g_VboHandle = 0, g_ElementsHandle = 0;

// Data
typedef enum GlfwClientApi
{
	GlfwClientApi_Unknown,
	GlfwClientApi_OpenGL,
	GlfwClientApi_Vulkan
} GlfwClientApi;
static GLFWwindow *g_Window = NULL; // Main window
static GlfwClientApi g_ClientApi = GlfwClientApi_Unknown;
static double g_Time = 0.0;
static bool g_MouseJustPressed[5] = {false, false, false, false, false};
static GLFWcursor *g_MouseCursors[ImGuiMouseCursor_COUNT] = {0};
static bool g_InstalledCallbacks = false;

// Chain GLFW callbacks: our callbacks will call the user's previously installed callbacks, if any.
static GLFWmousebuttonfun g_PrevUserCallbackMousebutton = NULL;
static GLFWscrollfun g_PrevUserCallbackScroll = NULL;
static GLFWkeyfun g_PrevUserCallbackKey = NULL;
static GLFWcharfun g_PrevUserCallbackChar = NULL;

void ImGui_ImplRaylib_UpdateMousePosAndButtons();
void ImGui_ImplRaylib_UpdateMouseCursor();

const char *ImGui_ImplRaylib_GetClipboardText()
{

	//return glfwGetClipboardString((GLFWwindow*)user_data);
	return GetClipboardText();
}

void ImGui_ImplRaylib_SetClipboardText(const char *text)
{
	//glfwSetClipboardString((GLFWwindow*)user_data, text);
	SetClipboardText(text);
}

void ImGui_ImplRaylib_MouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{

	if (g_PrevUserCallbackMousebutton != NULL)
		g_PrevUserCallbackMousebutton(window, button, action, mods);

	if (action == GLFW_PRESS && button >= 0 && button < IM_ARRAYSIZE(g_MouseJustPressed))
		g_MouseJustPressed[button] = true;
}

void ImGui_ImplRaylib_ScrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{
	if (g_PrevUserCallbackScroll != NULL)
		g_PrevUserCallbackScroll(window, xoffset, yoffset);

	ImGuiIO *io = igGetIO(); //ImGui::GetIO();
	io->MouseWheelH += (float)xoffset;
	io->MouseWheel += (float)yoffset;
}

void ImGui_ImplRaylib_KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (g_PrevUserCallbackKey != NULL)
		g_PrevUserCallbackKey(window, key, scancode, action, mods);

	ImGuiIO *io = igGetIO();
	if (action == GLFW_PRESS)
		io->KeysDown[key] = true;
	if (action == GLFW_RELEASE)
		io->KeysDown[key] = false;

	// Modifiers are not reliable across systems
	io->KeyCtrl = io->KeysDown[GLFW_KEY_LEFT_CONTROL] || io->KeysDown[GLFW_KEY_RIGHT_CONTROL];
	io->KeyShift = io->KeysDown[GLFW_KEY_LEFT_SHIFT] || io->KeysDown[GLFW_KEY_RIGHT_SHIFT];
	io->KeyAlt = io->KeysDown[GLFW_KEY_LEFT_ALT] || io->KeysDown[GLFW_KEY_RIGHT_ALT];
	io->KeySuper = io->KeysDown[GLFW_KEY_LEFT_SUPER] || io->KeysDown[GLFW_KEY_RIGHT_SUPER];
}

void ImGui_ImplRaylib_CharCallback(GLFWwindow *window, unsigned int c)
{
	if (g_PrevUserCallbackChar != NULL)
		g_PrevUserCallbackChar(window, c);

	ImGuiIO *io = igGetIO();
	ImGuiIO_AddInputCharacter(io, c);
}

bool ImGui_ImplRaylib_Init(GLFWwindow *window, bool install_callbacks, GlfwClientApi client_api)
{
	g_Window = window;
	g_Time = 0.0;

	// Setup back-end capabilities flags
	ImGuiIO *io = igGetIO();
	io->BackendFlags |= ImGuiBackendFlags_HasMouseCursors; // We can honor GetMouseCursor() values (optional)
	io->BackendFlags |= ImGuiBackendFlags_HasSetMousePos;  // We can honor io.WantSetMousePos requests (optional, rarely used)
	io->BackendPlatformName = "imgui_impl_glfw";

	// Keyboard mapping. ImGui will use those indices to peek into the io.KeysDown[] array.
	io->KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
	io->KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
	io->KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
	io->KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
	io->KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
	io->KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
	io->KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
	io->KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
	io->KeyMap[ImGuiKey_End] = GLFW_KEY_END;
	io->KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
	io->KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
	io->KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
	io->KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
	io->KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
	io->KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
	io->KeyMap[ImGuiKey_KeyPadEnter] = GLFW_KEY_KP_ENTER;
	io->KeyMap[ImGuiKey_A] = GLFW_KEY_A;
	io->KeyMap[ImGuiKey_C] = GLFW_KEY_C;
	io->KeyMap[ImGuiKey_V] = GLFW_KEY_V;
	io->KeyMap[ImGuiKey_X] = GLFW_KEY_X;
	io->KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
	io->KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

	io->SetClipboardTextFn = ImGui_ImplRaylib_SetClipboardText;
	io->GetClipboardTextFn = ImGui_ImplRaylib_GetClipboardText;
	io->ClipboardUserData = g_Window;
#if defined(_WIN32)
	io->ImeWindowHandle = (void *)glfwGetWin32Window(g_Window);
#endif

	g_MouseCursors[ImGuiMouseCursor_Arrow] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
	g_MouseCursors[ImGuiMouseCursor_TextInput] = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
	g_MouseCursors[ImGuiMouseCursor_ResizeAll] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR); // FIXME: GLFW doesn't have this.
	g_MouseCursors[ImGuiMouseCursor_ResizeNS] = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);
	g_MouseCursors[ImGuiMouseCursor_ResizeEW] = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
	g_MouseCursors[ImGuiMouseCursor_ResizeNESW] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR); // FIXME: GLFW doesn't have this.
	g_MouseCursors[ImGuiMouseCursor_ResizeNWSE] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR); // FIXME: GLFW doesn't have this.
	g_MouseCursors[ImGuiMouseCursor_Hand] = glfwCreateStandardCursor(GLFW_HAND_CURSOR);

	// Chain GLFW callbacks: our callbacks will call the user's previously installed callbacks, if any.
	g_PrevUserCallbackMousebutton = NULL;
	g_PrevUserCallbackScroll = NULL;
	g_PrevUserCallbackKey = NULL;
	g_PrevUserCallbackChar = NULL;
	if (install_callbacks)
	{
		g_InstalledCallbacks = true;
		g_PrevUserCallbackMousebutton = glfwSetMouseButtonCallback(window, ImGui_ImplRaylib_MouseButtonCallback);
		g_PrevUserCallbackScroll = glfwSetScrollCallback(window, ImGui_ImplRaylib_ScrollCallback);
		g_PrevUserCallbackKey = glfwSetKeyCallback(window, ImGui_ImplRaylib_KeyCallback);
		g_PrevUserCallbackChar = glfwSetCharCallback(window, ImGui_ImplRaylib_CharCallback);
	}

	g_ClientApi = client_api;
	return true;
}

bool ImGui_ImplRaylib_InitForOpenGL(GLFWwindow *window, bool install_callbacks)
{
	return ImGui_ImplRaylib_Init(window, install_callbacks, GlfwClientApi_OpenGL);
}

void ImGui_ImplRaylib_Shutdown()
{
	if (g_InstalledCallbacks)
	{
		glfwSetMouseButtonCallback(g_Window, g_PrevUserCallbackMousebutton);
		glfwSetScrollCallback(g_Window, g_PrevUserCallbackScroll);
		glfwSetKeyCallback(g_Window, g_PrevUserCallbackKey);
		glfwSetCharCallback(g_Window, g_PrevUserCallbackChar);
		g_InstalledCallbacks = false;
	}

	for (ImGuiMouseCursor cursor_n = 0; cursor_n < ImGuiMouseCursor_COUNT; cursor_n++)
	{
		glfwDestroyCursor(g_MouseCursors[cursor_n]);
		g_MouseCursors[cursor_n] = NULL;
	}
	g_ClientApi = GlfwClientApi_Unknown;
}

void ImGui_ImplRaylib_NewFrame()
{
	ImGuiIO *io = igGetIO();
	IM_ASSERT(ImFontAtlas_IsBuilt(io->Fonts) && "Font atlas not built! It is generally built by the renderer back-end. Missing call to renderer _NewFrame() function? e.g. ImGui_ImplOpenGL3_NewFrame().");

	// Setup display size (every frame to accommodate for window resizing)
	int w, h;
	int display_w, display_h;
	glfwGetWindowSize(g_Window, &w, &h);
	glfwGetFramebufferSize(g_Window, &display_w, &display_h);
	struct ImVec2 dispSize = {(float)w, (float)h};
	struct ImVec2 buffSize = {(float)display_w / w, (float)display_h / h};
	io->DisplaySize = dispSize;
	if (w > 0 && h > 0)
		io->DisplayFramebufferScale = buffSize;

	// Setup time step
	double current_time = glfwGetTime();
	io->DeltaTime = g_Time > 0.0 ? (float)(current_time - g_Time) : (float)(1.0f / 60.0f);
	g_Time = current_time;

	ImGui_ImplRaylib_UpdateMousePosAndButtons();
	ImGui_ImplRaylib_UpdateMouseCursor();

	// Todo: implement update game controllers (if enabled and available)
	// ImGui_ImplGlfw_UpdateGamepads();
}

void ImGui_ImplRaylib_UpdateMousePosAndButtons()
{
	// Update buttons
	ImGuiIO *io = igGetIO();
	for (int i = 0; i < IM_ARRAYSIZE(io->MouseDown); i++)
	{
		// If a mouse press event came, always pass it as "mouse held this frame", so we don't miss click-release events that are shorter than 1 frame.
		io->MouseDown[i] = g_MouseJustPressed[i] || glfwGetMouseButton(g_Window, i) != 0;
		g_MouseJustPressed[i] = false;
	}

	// Update mouse position
	const ImVec2 mouse_pos_backup = io->MousePos;
	struct ImVec2 mousePos = {-FLT_MAX, -FLT_MAX};
	io->MousePos = mousePos;
#ifdef __EMSCRIPTEN__
	const bool focused = true; // Emscripten
#else
	const bool focused = glfwGetWindowAttrib(g_Window, GLFW_FOCUSED) != 0;
#endif
	if (focused)
	{
		if (io->WantSetMousePos)
		{
			glfwSetCursorPos(g_Window, (double)mouse_pos_backup.x, (double)mouse_pos_backup.y);
		}
		else
		{
			double mouse_x, mouse_y;
			glfwGetCursorPos(g_Window, &mouse_x, &mouse_y);
			mousePos.x = (float)mouse_x;
			mousePos.y = (float)mouse_y;
			io->MousePos = mousePos;
		}
	}
}

void ImGui_ImplRaylib_UpdateMouseCursor()
{
	ImGuiIO *io = igGetIO();
	if ((io->ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange) || glfwGetInputMode(g_Window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
		return;

	ImGuiMouseCursor imgui_cursor = igGetMouseCursor();
	if (imgui_cursor == ImGuiMouseCursor_None || io->MouseDrawCursor)
	{
		// Hide OS mouse cursor if imgui is drawing it or if it wants no cursor
		glfwSetInputMode(g_Window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	}
	else
	{
		// Show OS mouse cursor
		// FIXME-PLATFORM: Unfocused windows seems to fail changing the mouse cursor with GLFW 3.2, but 3.3 works here.
		glfwSetCursor(g_Window, g_MouseCursors[imgui_cursor] ? g_MouseCursors[imgui_cursor] : g_MouseCursors[ImGuiMouseCursor_Arrow]);
		glfwSetInputMode(g_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}