//
// Created by Samuel on 1/2/2020.
//

#ifndef DECEMBERBLISS_LOGGUI_H
#define DECEMBERBLISS_LOGGUI_H
#include <imgui.h>

struct LogGui {

    ImGuiTextBuffer     Buf;
    ImGuiTextFilter     Filter;
    ImVector<int>       LineOffsets;        // Index to lines offset. We maintain this with AddLog() calls, allowing us to have a random access on lines
    bool                AutoScroll;     // Keep scrolling if already at the bottom

    LogGui();
    void Clear();
    void AddLog(const char* fmt, ...) IM_FMTARGS(2);
    void Draw(const char* title, bool* p_open = nullptr);
};


#endif //DECEMBERBLISS_LOGGUI_H
