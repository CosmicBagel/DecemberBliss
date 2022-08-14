#pragma once
#include <imgui.h>

struct Log_Gui {
    ImGuiTextBuffer buf;
    ImGuiTextFilter filter;
    ImVector<int>
        line_offsets;  // Index to lines offset. We maintain this with AddLog()
                       // calls, allowing us to have a random access on lines
    bool auto_scroll;  // Keep scrolling if already at the bottom

    Log_Gui();
    void clear();
    void add_log(const char* fmt, ...) IM_FMTARGS(2);
    void draw(const char* title, bool* p_open = nullptr);
};
