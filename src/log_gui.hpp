#pragma once
#include <imgui.h>

class LogGui {
   private:
    // NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
    static ImGuiTextBuffer buf;
    static ImGuiTextFilter filter;

    // Index to lines offset. We maintain this with AddLog()
    // calls, allowing us to have a random access on lines
    static ImVector<int> line_offsets;

    static bool auto_scroll;  // Keep scrolling if already at the bottom
    // NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)

    static void draw_text();

   public:
    static void clear();
    static void add_log(const char* fmt, ...) IM_FMTARGS(1);
    static void draw(const char* title, bool* p_open = nullptr);
};
