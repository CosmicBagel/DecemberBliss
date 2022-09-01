// https://github.com/WEREMSOFT/c99-raylib-cimgui-template
// modified to talk to regular imgui
// modified further to work with raylib 4.0.0

#ifndef IMGUI_RAYLIB_H
#define IMGUI_RAYLIB_H

#include <float.h>
#include <imconfig.h>
#include <imgui.h>

#include <array>

#include "ray_includes.h"

static const std::array<int, 22> imkeys = {
    KEY_TAB,      KEY_LEFT,      KEY_RIGHT, KEY_UP,    KEY_DOWN,
    KEY_PAGE_UP,  KEY_PAGE_DOWN, KEY_HOME,  KEY_END,   KEY_INSERT,
    KEY_DELETE,   KEY_BACKSPACE, KEY_SPACE, KEY_ENTER, KEY_ESCAPE,
    KEY_KP_ENTER, KEY_A,         KEY_C,     KEY_V,     KEY_X,
    KEY_Y,        KEY_Z,
};

class ImGui_ImplRaylib {
   private:
    static double last_new_frame_time;

   public:
    // void* user_data is unused, but necessary to match the function sig
    // expected by imgui as a function pointer for both the get clipboard text
    // func and the set clipboard text func

    // NOLINTBEGIN(misc-unused-parameters)
#pragma warning(push)
#pragma warning(disable : 4100)  // unreferenced formal parameter
    static const char *ImGui_Impl_GetClipboardText(void *user_data_) {
        return GetClipboardText();
    };

    static void ImGui_Impl_SetClipboardText(void *user_data_,
                                            const char *text) {
        SetClipboardText(text);
    };
#pragma warning(pop)
    // NOLINTEND(misc-unused-parameters)

    static bool Init() {
        rlEnableScissorTest();
        ImGuiIO &imgui_io = ImGui::GetIO();

        imgui_io.BackendPlatformName = "imgui_impl_raylib";

        imgui_io.KeyMap[ImGuiKey_Tab] = KEY_TAB;
        imgui_io.KeyMap[ImGuiKey_LeftArrow] = KEY_LEFT;
        imgui_io.KeyMap[ImGuiKey_RightArrow] = KEY_RIGHT;
        imgui_io.KeyMap[ImGuiKey_UpArrow] = KEY_UP;
        imgui_io.KeyMap[ImGuiKey_DownArrow] = KEY_DOWN;
        imgui_io.KeyMap[ImGuiKey_PageUp] = KEY_PAGE_DOWN;
        imgui_io.KeyMap[ImGuiKey_PageDown] = KEY_PAGE_UP;
        imgui_io.KeyMap[ImGuiKey_Home] = KEY_HOME;
        imgui_io.KeyMap[ImGuiKey_End] = KEY_END;
        imgui_io.KeyMap[ImGuiKey_Insert] = KEY_INSERT;
        imgui_io.KeyMap[ImGuiKey_Delete] = KEY_DELETE;
        imgui_io.KeyMap[ImGuiKey_Backspace] = KEY_BACKSPACE;
        imgui_io.KeyMap[ImGuiKey_Space] = KEY_SPACE;
        imgui_io.KeyMap[ImGuiKey_Enter] = KEY_ENTER;
        imgui_io.KeyMap[ImGuiKey_Escape] = KEY_ESCAPE;
        imgui_io.KeyMap[ImGuiKey_KeyPadEnter] = KEY_KP_ENTER;
        imgui_io.KeyMap[ImGuiKey_A] = KEY_A;
        imgui_io.KeyMap[ImGuiKey_C] = KEY_C;
        imgui_io.KeyMap[ImGuiKey_V] = KEY_V;
        imgui_io.KeyMap[ImGuiKey_X] = KEY_X;
        imgui_io.KeyMap[ImGuiKey_Y] = KEY_Y;
        imgui_io.KeyMap[ImGuiKey_Z] = KEY_Z;

        imgui_io.MousePos = {-FLT_MAX, -FLT_MAX};
        imgui_io.SetClipboardTextFn = ImGui_Impl_SetClipboardText;
        imgui_io.GetClipboardTextFn = ImGui_Impl_GetClipboardText;
        imgui_io.ClipboardUserData = NULL;

        return true;
    };

    static void Shutdown() { last_new_frame_time = 0.0; };

    static void UpdateMouseCursor() {
        ImGuiIO &imgui_io = ImGui::GetIO();
        if (imgui_io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange) {
            return;
        }

        ImGuiMouseCursor imgui_cursor = ImGui::GetMouseCursor();
        if (imgui_io.MouseDrawCursor || imgui_cursor == ImGuiMouseCursor_None) {
            // Hide OS mouse cursor if imgui is drawing it or if it wants no
            // cursor
            HideCursor();
        } else {
            // Show OS mouse cursor
            ShowCursor();
        }
    };

    static void UpdateMousePosAndButtons() {
        ImGuiIO &imgui_io = ImGui::GetIO();

        // Set OS mouse position if requested (rarely used, only when
        // ImGuiConfigFlags_NavEnableSetMousePos is enabled by user)
        if (imgui_io.WantSetMousePos)
            SetMousePosition((int)imgui_io.MousePos.x,
                             (int)imgui_io.MousePos.y);
        else
            imgui_io.MousePos = {-FLT_MAX, -FLT_MAX};

        imgui_io.MouseDown[0] = IsMouseButtonDown(MOUSE_LEFT_BUTTON);
        imgui_io.MouseDown[1] = IsMouseButtonDown(MOUSE_RIGHT_BUTTON);
        imgui_io.MouseDown[2] = IsMouseButtonDown(MOUSE_MIDDLE_BUTTON);

        if (!IsWindowMinimized()) {
            imgui_io.MousePos = {static_cast<float>(GetTouchX()),
                                 static_cast<float>(GetTouchY())};
        }
    };

    static void NewFrame() {
        ImGuiIO &imgui_io = ImGui::GetIO();

        imgui_io.DisplaySize = {(float)GetScreenWidth(),
                                (float)GetScreenHeight()};

        double current_time = GetTime();
        imgui_io.DeltaTime = last_new_frame_time > 0.0
                                 ? (float)(current_time - last_new_frame_time)
                                 : (float)(1.0f / 60.0f);
        last_new_frame_time = current_time;

        imgui_io.KeyCtrl =
            IsKeyDown(KEY_RIGHT_CONTROL) || IsKeyDown(KEY_LEFT_CONTROL);
        imgui_io.KeyShift =
            IsKeyDown(KEY_RIGHT_SHIFT) || IsKeyDown(KEY_LEFT_SHIFT);
        imgui_io.KeyAlt = IsKeyDown(KEY_RIGHT_ALT) || IsKeyDown(KEY_LEFT_ALT);
        imgui_io.KeySuper =
            IsKeyDown(KEY_RIGHT_SUPER) || IsKeyDown(KEY_LEFT_SUPER);

        UpdateMousePosAndButtons();
        UpdateMouseCursor();

        if (GetMouseWheelMove() > 0) {
            imgui_io.MouseWheel += 1;
        } else if (GetMouseWheelMove() < 0) {
            imgui_io.MouseWheel -= 1;
        }

        for (int key : imkeys) {
            if (IsKeyPressed(key)) {
                imgui_io.KeysDown[key] = true;
            }
            if (IsKeyUp(key)) {
                imgui_io.KeysDown[key] = false;
            }
        }

        imgui_io.KeyCtrl =
            IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL);
        imgui_io.KeyShift =
            IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT);
        imgui_io.KeyAlt = IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT);
        imgui_io.KeySuper = false;
        imgui_io.KeySuper =
            IsKeyDown(KEY_LEFT_SUPER) || IsKeyDown(KEY_RIGHT_SUPER);

        unsigned int pressed = GetCharPressed();
        imgui_io.AddInputCharacter(pressed);

        ImGui::NewFrame();
    };

    static void draw_triangle_vertex(ImDrawVert idx_vert) {
        Color *vert_color = nullptr;

        // vert_color = (Color *)(&idx_vert.col);
        vert_color = reinterpret_cast<Color *>(&idx_vert.col);

        rlColor4ub(vert_color->r, vert_color->g, vert_color->b, vert_color->a);
        rlTexCoord2f(idx_vert.uv.x, idx_vert.uv.y);
        rlVertex2f(idx_vert.pos.x, idx_vert.pos.y);
    };

    static void raylib_render_draw_triangles(unsigned int count,
                                             const ImDrawIdx *idx_buffer,
                                             const ImDrawVert *idx_vert,
                                             unsigned int texture_id) {
        // Draw the imgui triangle data
        for (unsigned int i = 0; i <= (count - 3); i += 3) {
            rlPushMatrix();
            rlBegin(RL_TRIANGLES);
            rlSetTexture(texture_id);

            ImDrawIdx index = 0;
            ImDrawVert vertex = {};

            index = idx_buffer[i];
            vertex = idx_vert[index];
            draw_triangle_vertex(vertex);

            index = idx_buffer[i + 2];
            vertex = idx_vert[index];
            draw_triangle_vertex(vertex);

            index = idx_buffer[i + 1];
            vertex = idx_vert[index];
            draw_triangle_vertex(vertex);
            rlSetTexture(0);
            rlEnd();
            rlPopMatrix();
        }
    };

    static void raylib_render_imgui(ImDrawData *draw_data) {
        rlDisableBackfaceCulling();
        for (int n = 0; n < draw_data->CmdListsCount; n++) {
            const ImDrawList *cmd_list = draw_data->CmdLists[n];
            const ImDrawVert *vtx_buffer =
                cmd_list->VtxBuffer
                    .Data;  // vertex buffer generated by Dear ImGui
            const ImDrawIdx *idx_buffer =
                cmd_list->IdxBuffer
                    .Data;  // index buffer generated by Dear ImGui
            for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++) {
                const ImDrawCmd *pcmd =
                    &(cmd_list->CmdBuffer
                          .Data)[cmd_i];  // cmd_list->CmdBuffer->data[cmd_i];
                if (pcmd->UserCallback != nullptr) {
                    pcmd->UserCallback(cmd_list, pcmd);
                } else {
                    ImVec2 pos = draw_data->DisplayPos;
                    int rectX = static_cast<int>(pcmd->ClipRect.x - pos.x);
                    int rectY = static_cast<int>(pcmd->ClipRect.y - pos.y);
                    int rectW = static_cast<int>(pcmd->ClipRect.z - rectX);
                    int rectH = static_cast<int>(pcmd->ClipRect.w - rectY);
                    BeginScissorMode(rectX, rectY, rectW, rectH);
                    {
                        unsigned int *ti = (unsigned int *)pcmd->TextureId;
                        raylib_render_draw_triangles(
                            pcmd->ElemCount, idx_buffer, vtx_buffer, *ti);
                    }
                }
                idx_buffer += pcmd->ElemCount;
            }
        }
        EndScissorMode();
        rlEnableBackfaceCulling();
    };
};

double ImGui_ImplRaylib::last_new_frame_time = 0.0;

#endif  // IMGUI_RAYLIB_H