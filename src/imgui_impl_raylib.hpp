// https://github.com/WEREMSOFT/c99-raylib-cimgui-template
// modified to talk to regular imgui
// modified further to work with raylib 4.0.0

#ifndef IMGUI_RAYLIB_H
#define IMGUI_RAYLIB_H

#include <float.h>
#include <imconfig.h>
#include <imgui.h>

#include <array>

#include "ray_includes.hpp"

class ImguiImplRaylib {
   private:
    static double last_new_frame_time;
    static const std::array<int, 22> imkeys;

   public:
    static const char *imgui_get_clipboard_text(void *user_data);
    static void imgui_set_clipboard_text(void *user_data, const char *text);

    static bool init();
    static void shutdown();

    static void update_mouse_cursor();
    static void update_mouse_pos_and_buttons();

    static void new_frame();

    static void draw_triangle_vertex(ImDrawVert idx_vert);
    static void raylib_render_draw_triangles(unsigned int count,
                                             const ImDrawIdx *idx_buffer,
                                             const ImDrawVert *idx_vert,
                                             unsigned int texture_id);
    static void raylib_render_imgui(ImDrawData *draw_data);
};

#endif  // IMGUI_RAYLIB_H