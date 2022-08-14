// https://github.com/WEREMSOFT/c99-raylib-cimgui-template
// modified to talk to regular imgui
// modified further to work with raylib 4.0.0

#ifndef IMGUI_RAYLIB_H
#define IMGUI_RAYLIB_H

#include <float.h>
#include <imconfig.h>
#include <imgui.h>

#include "ray_includes.h"

static double last_new_frame_time = 0.0;

static const int imkeys[] = {
    KEY_TAB,      KEY_LEFT,      KEY_RIGHT, KEY_UP,    KEY_DOWN,
    KEY_PAGE_UP,  KEY_PAGE_DOWN, KEY_HOME,  KEY_END,   KEY_INSERT,
    KEY_DELETE,   KEY_BACKSPACE, KEY_SPACE, KEY_ENTER, KEY_ESCAPE,
    KEY_KP_ENTER, KEY_A,         KEY_C,     KEY_V,     KEY_X,
    KEY_Y,        KEY_Z,
};

static const char *ImGui_ImplRaylib_GetClipboardText(void *_) {
    return GetClipboardText();
}

static void ImGui_ImplRaylib_SetClipboardText(void *_, const char *text) {
    SetClipboardText(text);
}

bool ImGui_ImplRaylib_Init() {
    rlEnableScissorTest();
    ImGuiIO &io = ImGui::GetIO();

    io.BackendPlatformName = "imgui_impl_raylib";

    io.KeyMap[ImGuiKey_Tab] = KEY_TAB;
    io.KeyMap[ImGuiKey_LeftArrow] = KEY_LEFT;
    io.KeyMap[ImGuiKey_RightArrow] = KEY_RIGHT;
    io.KeyMap[ImGuiKey_UpArrow] = KEY_UP;
    io.KeyMap[ImGuiKey_DownArrow] = KEY_DOWN;
    io.KeyMap[ImGuiKey_PageUp] = KEY_PAGE_DOWN;
    io.KeyMap[ImGuiKey_PageDown] = KEY_PAGE_UP;
    io.KeyMap[ImGuiKey_Home] = KEY_HOME;
    io.KeyMap[ImGuiKey_End] = KEY_END;
    io.KeyMap[ImGuiKey_Insert] = KEY_INSERT;
    io.KeyMap[ImGuiKey_Delete] = KEY_DELETE;
    io.KeyMap[ImGuiKey_Backspace] = KEY_BACKSPACE;
    io.KeyMap[ImGuiKey_Space] = KEY_SPACE;
    io.KeyMap[ImGuiKey_Enter] = KEY_ENTER;
    io.KeyMap[ImGuiKey_Escape] = KEY_ESCAPE;
    io.KeyMap[ImGuiKey_KeyPadEnter] = KEY_KP_ENTER;
    io.KeyMap[ImGuiKey_A] = KEY_A;
    io.KeyMap[ImGuiKey_C] = KEY_C;
    io.KeyMap[ImGuiKey_V] = KEY_V;
    io.KeyMap[ImGuiKey_X] = KEY_X;
    io.KeyMap[ImGuiKey_Y] = KEY_Y;
    io.KeyMap[ImGuiKey_Z] = KEY_Z;

    io.MousePos = {-FLT_MAX, -FLT_MAX};
    io.SetClipboardTextFn = ImGui_ImplRaylib_SetClipboardText;
    io.GetClipboardTextFn = ImGui_ImplRaylib_GetClipboardText;
    io.ClipboardUserData = NULL;

    return true;
}

void ImGui_ImplRaylib_Shutdown() { last_new_frame_time = 0.0; }

static void ImGui_ImplRaylib_UpdateMouseCursor() {
    ImGuiIO &io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange) return;

    ImGuiMouseCursor imgui_cursor = ImGui::GetMouseCursor();
    if (io.MouseDrawCursor || imgui_cursor == ImGuiMouseCursor_None) {
        // Hide OS mouse cursor if imgui is drawing it or if it wants no cursor
        HideCursor();
    } else {
        // Show OS mouse cursor
        ShowCursor();
    }
}

static void ImGui_ImplRaylib_UpdateMousePosAndButtons() {
    ImGuiIO &io = ImGui::GetIO();

    // Set OS mouse position if requested (rarely used, only when
    // ImGuiConfigFlags_NavEnableSetMousePos is enabled by user)
    if (io.WantSetMousePos)
        SetMousePosition((int)io.MousePos.x, (int)io.MousePos.y);
    else
        io.MousePos = {-FLT_MAX, -FLT_MAX};

    io.MouseDown[0] = IsMouseButtonDown(MOUSE_LEFT_BUTTON);
    io.MouseDown[1] = IsMouseButtonDown(MOUSE_RIGHT_BUTTON);
    io.MouseDown[2] = IsMouseButtonDown(MOUSE_MIDDLE_BUTTON);

    if (!IsWindowMinimized()) {
        io.MousePos = {(float)GetTouchX(), (float)GetTouchY()};
    }
}

void ImGui_ImplRaylib_NewFrame() {
    ImGuiIO &io = ImGui::GetIO();

    io.DisplaySize = {(float)GetScreenWidth(), (float)GetScreenHeight()};

    double current_time = GetTime();
    io.DeltaTime = last_new_frame_time > 0.0
                       ? (float)(current_time - last_new_frame_time)
                       : (float)(1.0f / 60.0f);
    last_new_frame_time = current_time;

    io.KeyCtrl = IsKeyDown(KEY_RIGHT_CONTROL) || IsKeyDown(KEY_LEFT_CONTROL);
    io.KeyShift = IsKeyDown(KEY_RIGHT_SHIFT) || IsKeyDown(KEY_LEFT_SHIFT);
    io.KeyAlt = IsKeyDown(KEY_RIGHT_ALT) || IsKeyDown(KEY_LEFT_ALT);
    io.KeySuper = IsKeyDown(KEY_RIGHT_SUPER) || IsKeyDown(KEY_LEFT_SUPER);

    ImGui_ImplRaylib_UpdateMousePosAndButtons();
    ImGui_ImplRaylib_UpdateMouseCursor();

    if (GetMouseWheelMove() > 0)
        io.MouseWheel += 1;
    else if (GetMouseWheelMove() < 0)
        io.MouseWheel -= 1;

    for (int key : imkeys) {
        if (IsKeyPressed(key)) {
            io.KeysDown[key] = true;
        }
        if (IsKeyUp(key)) {
            io.KeysDown[key] = false;
        }
    }

    io.KeyCtrl = IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL);
    io.KeyShift = IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT);
    io.KeyAlt = IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT);
    io.KeySuper = false;
    io.KeySuper = IsKeyDown(KEY_LEFT_SUPER) || IsKeyDown(KEY_RIGHT_SUPER);

    unsigned int pressed = GetCharPressed();
    io.AddInputCharacter(pressed);

    ImGui::NewFrame();
}

void draw_triangle_vertex(ImDrawVert idx_vert) {
    Color *c;
    c = (Color *)&idx_vert.col;
    rlColor4ub(c->r, c->g, c->b, c->a);
    rlTexCoord2f(idx_vert.uv.x, idx_vert.uv.y);
    rlVertex2f(idx_vert.pos.x, idx_vert.pos.y);
}

void raylib_render_draw_triangles(unsigned int count,
                                  const ImDrawIdx *idx_buffer,
                                  const ImDrawVert *idx_vert,
                                  unsigned int texture_id) {
    // Draw the imgui triangle data
    for (unsigned int i = 0; i <= (count - 3); i += 3) {
        rlPushMatrix();
        rlBegin(RL_TRIANGLES);
        rlSetTexture(texture_id);

        ImDrawIdx index;
        ImDrawVert vertex;

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
}

void raylib_render_imgui(ImDrawData *draw_data) {
    rlDisableBackfaceCulling();
    for (int n = 0; n < draw_data->CmdListsCount; n++) {
        const ImDrawList *cmd_list = draw_data->CmdLists[n];
        const ImDrawVert *vtx_buffer =
            cmd_list->VtxBuffer.Data;  // vertex buffer generated by Dear ImGui
        const ImDrawIdx *idx_buffer =
            cmd_list->IdxBuffer.Data;  // index buffer generated by Dear ImGui
        for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++) {
            const ImDrawCmd *pcmd =
                &(cmd_list->CmdBuffer
                      .Data)[cmd_i];  // cmd_list->CmdBuffer->data[cmd_i];
            if (pcmd->UserCallback) {
                pcmd->UserCallback(cmd_list, pcmd);
            } else {
                ImVec2 pos = draw_data->DisplayPos;
                int rectX = (int)(pcmd->ClipRect.x - pos.x);
                int rectY = (int)(pcmd->ClipRect.y - pos.y);
                int rectW = (int)(pcmd->ClipRect.z - rectX);
                int rectH = (int)(pcmd->ClipRect.w - rectY);
                BeginScissorMode(rectX, rectY, rectW, rectH);
                {
                    unsigned int *ti = (unsigned int *)pcmd->TextureId;
                    raylib_render_draw_triangles(pcmd->ElemCount, idx_buffer,
                                                 vtx_buffer, *ti);
                }
            }
            idx_buffer += pcmd->ElemCount;
        }
    }
    EndScissorMode();
    rlEnableBackfaceCulling();
}

#endif  // IMGUI_RAYLIB_H