
typedef struct GLFWwindow GLFWwindow;
#include <stdbool.h>

//-----------glfw

bool ImGui_ImplRaylib_InitForOpenGL(GLFWwindow* window, bool install_callbacks);
bool ImGui_ImplRaylib_InitForVulkan(GLFWwindow* window, bool install_callbacks);
void ImGui_ImplRaylib_Shutdown();
void ImGui_ImplRaylib_NewFrame();

// InitXXX function with 'install_callbacks=true': install GLFW callbacks. They will call user's previously installed callbacks, if any.
// InitXXX function with 'install_callbacks=false': do not install GLFW callbacks. You will need to call them yourself from your own GLFW callbacks.
void ImGui_ImplRaylib_MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void ImGui_ImplRaylib_ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void ImGui_ImplRaylib_KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void ImGui_ImplRaylib_CharCallback(GLFWwindow* window, unsigned int c);


