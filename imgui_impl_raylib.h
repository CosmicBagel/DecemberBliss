
typedef struct GLFWwindow GLFWwindow;
#include <stdbool.h>

//-----------glfw

bool ImGui_ImplGlfw_InitForOpenGL(GLFWwindow* window, bool install_callbacks);
bool ImGui_ImplGlfw_InitForVulkan(GLFWwindow* window, bool install_callbacks);
void ImGui_ImplGlfw_Shutdown();
void ImGui_ImplGlfw_NewFrame();

// InitXXX function with 'install_callbacks=true': install GLFW callbacks. They will call user's previously installed callbacks, if any.
// InitXXX function with 'install_callbacks=false': do not install GLFW callbacks. You will need to call them yourself from your own GLFW callbacks.
void ImGui_ImplGlfw_MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void ImGui_ImplGlfw_ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void ImGui_ImplGlfw_KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void ImGui_ImplGlfw_CharCallback(GLFWwindow* window, unsigned int c);


//-----------opengl

// Backend API
bool     ImGui_ImplOpenGL3_Init(const char* glsl_version);
void     ImGui_ImplOpenGL3_Shutdown();
void     ImGui_ImplOpenGL3_NewFrame();
void     ImGui_ImplOpenGL3_RenderDrawData(ImDrawData* draw_data);

// (Optional) Called by Init/NewFrame/Shutdown
bool     ImGui_ImplOpenGL3_CreateFontsTexture();
void     ImGui_ImplOpenGL3_DestroyFontsTexture();
bool     ImGui_ImplOpenGL3_CreateDeviceObjects();
void     ImGui_ImplOpenGL3_DestroyDeviceObjects();

#define IMGUI_IMPL_OPENGL_LOADER_GLAD