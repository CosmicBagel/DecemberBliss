//originally from, converted to C with mild modifications
//https://github.com/Ushio/raylibImGui/blob/master/src/imguiintegration.hpp

#include "string.h"
#include "stdlib.h"
#include "assert.h"

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include "cimgui.h"
//not in cimgui, but needed
#define ImDrawCallback_ResetRenderState (ImDrawCallback)(-1)
#define IM_ASSERT(_EXPR) assert(_EXPR)

#include "rlgl.h"

#include "imgui_integration.h"

/*
notes
- LOG_INFO is too noisy because of Mesh upload. Please use SetTraceLogLevel(LOG_WARNING);
- SetupState changes global state per rendering
- Texture2D * == ImTextureID
// needs UnloadMesh
*/

static const int imKeys[] = {
    KEY_TAB,
    KEY_LEFT,
    KEY_RIGHT,
    KEY_UP,
    KEY_DOWN,
    KEY_PAGE_UP,
    KEY_PAGE_DOWN,
    KEY_HOME,
    KEY_END,
    KEY_INSERT,
    KEY_DELETE,
    KEY_BACKSPACE,
    KEY_SPACE,
    KEY_ENTER,
    KEY_ESCAPE,
    KEY_KP_ENTER,
    KEY_A,
    KEY_C,
    KEY_V,
    KEY_X,
    KEY_Y,
    KEY_Z};
static const int imKeysCount = sizeof(imKeys) / sizeof(int);

//user_data is requried, as the function is called by imgui, and will try to pass
//the data into the function
const char *GetClipboardTextForImGui(void *user_data)
{
    (void)user_data; //hack to get rid of unused variable warning
    return GetClipboardText();
}

void SetClipboardTextForImGui(void *user_data, const char *text)
{
    (void)user_data;
    SetClipboardText(text);
}

void SetupState(int fb_width, int fb_height)
{
    rlglDraw();

    // Setup render state: alpha-blending enabled, no face culling, no depth testing, scissor enabled, vertex/texcoord/color pointers, polygon fill.
    BeginBlendMode(BLEND_ALPHA);

    rlEnableScissorTest();
    rlScissor(0, 0, fb_width, fb_height);

    rlDisableBackfaceCulling();
    rlDisableDepthTest();
    rlDisableBackfaceCulling();
    rlDisableWireMode();
    rlViewport(0, 0, fb_width, fb_height);
}

Mesh buildMesh(Vector3 *vertices, Vector2 *texCoords, Color *colors, unsigned int vertexCount)
{
    Mesh mesh;
    mesh.vboId = (unsigned int *)RL_CALLOC(20, sizeof(unsigned int));

    mesh.vertexCount = vertexCount;
    mesh.vertices = (float *)RL_MALLOC(mesh.vertexCount * sizeof(Vector3));
    memcpy(mesh.vertices, vertices, vertexCount * sizeof(Vector3));

    mesh.texcoords = (float *)RL_MALLOC(mesh.vertexCount * sizeof(Vector2));
    memcpy(mesh.texcoords, texCoords, vertexCount * sizeof(Vector2));

    mesh.colors = (unsigned char *)RL_MALLOC(mesh.vertexCount * sizeof(Color));
    memcpy(mesh.colors, colors, vertexCount * sizeof(Color));

    rlLoadMesh(&mesh, false);
    return mesh;
}

void Draw(ImDrawData *draw_data)
{
    // Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
    int fb_width = (int)(draw_data->DisplaySize.x * draw_data->FramebufferScale.x);
    int fb_height = (int)(draw_data->DisplaySize.y * draw_data->FramebufferScale.y);
    if (fb_width == 0 || fb_height == 0)
        return;

    // Assumptions
    // - FramebufferScale.xy == 1
    // - DisplaySize.xy == screen size

    Camera2D camera = {Vector2Zero(), Vector2Zero(), 0.0f, 0.0f};
    camera.zoom = 1.0f;
    BeginMode2D(camera);

    SetupState(fb_width, fb_height);

    // Will project scissor/clipping rectangles into framebuffer space
    ImVec2 clip_off = draw_data->DisplayPos;         // (0,0) unless using multi-viewports
    ImVec2 clip_scale = draw_data->FramebufferScale; // (1,1) unless using retina display which are often (2,2)

    // Render command lists
    for (int n = 0; n < draw_data->CmdListsCount; n++)
    {
        const ImDrawList *cmd_list = draw_data->CmdLists[n];
        const ImDrawVert *vtx_buffer = cmd_list->VtxBuffer.Data;
        const ImDrawIdx *idx_buffer = cmd_list->IdxBuffer.Data;

        for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
        {
            unsigned int elemCount = cmd_list->CmdBuffer.Data->ElemCount;
            const ImDrawCmd *pcmd = &cmd_list->CmdBuffer.Data[cmd_i];
            Vector3 *vertices = malloc(elemCount * sizeof(Vector3));
            Vector2 *texCoords = malloc(elemCount * sizeof(Vector2));
            Color *colors = malloc(elemCount * sizeof(Color));

            if (pcmd->UserCallback)
            {
                // User callback, registered via ImDrawList::AddCallback()
                // (ImDrawCallback_ResetRenderState is a special callback value used by the user to request the renderer to reset render state.)
                if (pcmd->UserCallback == ImDrawCallback_ResetRenderState)
                {
                    SetupState(fb_width, fb_height);
                }
                else
                    pcmd->UserCallback(cmd_list, pcmd);
            }
            else
            {
                // Project scissor/clipping rectangles into framebuffer space
                ImVec4 clip_rect;
                clip_rect.x = (pcmd->ClipRect.x - clip_off.x) * clip_scale.x;
                clip_rect.y = (pcmd->ClipRect.y - clip_off.y) * clip_scale.y;
                clip_rect.z = (pcmd->ClipRect.z - clip_off.x) * clip_scale.x;
                clip_rect.w = (pcmd->ClipRect.w - clip_off.y) * clip_scale.y;

                if (clip_rect.x < fb_width && clip_rect.y < fb_height && clip_rect.z >= 0.0f && clip_rect.w >= 0.0f)
                {
                    // Apply scissor/clipping rectangle
                    rlScissor((int)clip_rect.x, (int)(fb_height - clip_rect.w), (int)(clip_rect.z - clip_rect.x), (int)(clip_rect.w - clip_rect.y));

                    //MeshBuilder builder;

                    for (int i = 0; i < (int)pcmd->ElemCount; ++i)
                    {
                        ImDrawVert vert = vtx_buffer[idx_buffer[i]];
                        vertices[i] = (Vector3){vert.pos.x, vert.pos.y, 0.0f};
                        texCoords[i] = (Vector2){vert.uv.x, vert.uv.y};
                        colors[i] = (Color){
                            (vert.col >> 0) & 0xFF,
                            (vert.col >> 8) & 0xFF,
                            (vert.col >> 16) & 0xFF,
                            (vert.col >> 24) & 0xFF};
                    }
                    Mesh mesh = buildMesh(vertices, texCoords, colors, elemCount);

                    Material mat = LoadMaterialDefault();
                    SetMaterialTexture(&mat, MAP_DIFFUSE, *(Texture2D *)pcmd->TextureId);
                    rlDrawMesh(mesh, mat, MatrixIdentity());
                    SetMaterialTexture(&mat, MAP_DIFFUSE, GetTextureDefault());
                    UnloadMaterial(mat);

                    rlUnloadMesh(mesh);
                }
            }
            idx_buffer += pcmd->ElemCount;
        }
    }

    EndMode2D();

    rlDisableScissorTest();
}

// After raylib initialization
void ImGuiInitialize()
{
    ImGuiIO *io = igGetIO();
    io->BackendRendererName = "raylib";

    io->KeyMap[ImGuiKey_Tab] = KEY_TAB;
    io->KeyMap[ImGuiKey_LeftArrow] = KEY_LEFT;
    io->KeyMap[ImGuiKey_RightArrow] = KEY_RIGHT;
    io->KeyMap[ImGuiKey_UpArrow] = KEY_UP;
    io->KeyMap[ImGuiKey_DownArrow] = KEY_DOWN;
    io->KeyMap[ImGuiKey_PageUp] = KEY_PAGE_UP;
    io->KeyMap[ImGuiKey_PageDown] = KEY_PAGE_DOWN;
    io->KeyMap[ImGuiKey_Home] = KEY_HOME;
    io->KeyMap[ImGuiKey_End] = KEY_END;
    io->KeyMap[ImGuiKey_Insert] = KEY_INSERT;
    io->KeyMap[ImGuiKey_Delete] = KEY_DELETE;
    io->KeyMap[ImGuiKey_Backspace] = KEY_BACKSPACE;
    io->KeyMap[ImGuiKey_Space] = KEY_SPACE;
    io->KeyMap[ImGuiKey_Enter] = KEY_ENTER;
    io->KeyMap[ImGuiKey_Escape] = KEY_ESCAPE;
    io->KeyMap[ImGuiKey_KeyPadEnter] = KEY_KP_ENTER;
    io->KeyMap[ImGuiKey_A] = KEY_A;
    io->KeyMap[ImGuiKey_C] = KEY_C;
    io->KeyMap[ImGuiKey_V] = KEY_V;
    io->KeyMap[ImGuiKey_X] = KEY_X;
    io->KeyMap[ImGuiKey_Y] = KEY_Y;
    io->KeyMap[ImGuiKey_Z] = KEY_Z;

    io->SetClipboardTextFn = SetClipboardTextForImGui;
    io->GetClipboardTextFn = GetClipboardTextForImGui;

    igStyleColorsDark(igGetStyle());

    // Build texture atlas
    unsigned char *pixels;
    int width, height;
    int bytesPerPixel;
    ImFontAtlas_GetTexDataAsRGBA32(io->Fonts,
                                   &pixels, &width, &height,
                                   &bytesPerPixel); // Load as RGBA 32-bits (75% of the memory is wasted, but
                                                    // default font is so small) because it is more likely to be
                                                    // compatible with user's existing shaders. If your
                                                    // ImTextureId represent a higher-level concept than just a
                                                    // GL texture id, consider calling GetTexDataAsAlpha8()
                                                    // instead to save on GPU memory.

    Image image = LoadImagePro(pixels, width, height, UNCOMPRESSED_R8G8B8A8);
    Texture2D theTexture = LoadTextureFromImage(image);
    SetTextureFilter(theTexture, FILTER_BILINEAR);
    SetTextureWrap(theTexture, WRAP_CLAMP);

    Texture2D *fontTex = malloc(sizeof(Texture2D));
    (*fontTex) = theTexture;
    io->Fonts->TexID = fontTex;
}

void BeginImGui()
{
    ImGuiIO *io = igGetIO();
    IM_ASSERT(ImFontAtlas_IsBuilt(io->Fonts) &&
              "Font atlas not built! It is generally built by the renderer back-end. Missing call to renderer _NewFrame() function? e.g. ImGui_ImplOpenGL3_NewFrame().");

    // Setup display size (every frame to accommodate for window resizing)
    io->DisplaySize = (ImVec2){(float)GetScreenWidth(), (float)GetScreenHeight()};
    io->DisplayFramebufferScale = (ImVec2){1, 1};

    // Setup time step
    // io->DeltaTime = g_Time > 0.0 ? (float)(current_time - g_Time) : (float)(1.0f / 60.0f);
    float frameTime = GetFrameTime();
    io->DeltaTime = frameTime;
    io->MousePos = (ImVec2){(float)GetMouseX(), (float)GetMouseY()};

    io->MouseDown[0] = IsMouseButtonDown(MOUSE_LEFT_BUTTON);
    io->MouseDown[1] = IsMouseButtonDown(MOUSE_RIGHT_BUTTON);
    io->MouseDown[2] = IsMouseButtonDown(MOUSE_MIDDLE_BUTTON);

    io->MouseWheel += (float)GetMouseWheelMove();

    for (int keyInd = 0; keyInd < imKeysCount; keyInd++)
    {
        int key = imKeys[keyInd];
        if (IsKeyPressed(key))
        {
            io->KeysDown[key] = true;
        }
        if (IsKeyUp(key))
        {
            io->KeysDown[key] = false;
        }
    }

    io->KeyCtrl = IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL);
    io->KeyShift = IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT);
    io->KeyAlt = IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT);
    io->KeySuper = IsKeyDown(KEY_LEFT_SUPER) || IsKeyDown(KEY_RIGHT_SUPER);

    ImGuiIO_AddInputCharacter(io, GetKeyPressed());

    igNewFrame();
}

void EndImGui()
{
    igRender();
    Draw(igGetDrawData());
    igEndFrame();
}

void DestroyImGui()
{
    ImGuiIO *io = igGetIO();
    free(io->Fonts->TexID);
}