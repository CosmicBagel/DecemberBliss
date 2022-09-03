#include "log_gui.hpp"

// Usage:
//  static ExampleAppLog my_log;
//  my_log.AddLog("Hello %d world\n", 123);
//  my_log.Draw("title");

// NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
bool LogGui::auto_scroll = true;
ImGuiTextBuffer LogGui::buf = {};
ImGuiTextFilter LogGui::filter = {};
ImVector<int> LogGui::line_offsets = {};
// NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)

void LogGui::clear() {
    buf.clear();
    line_offsets.clear();
    line_offsets.push_back(0);
}

void LogGui::add_log(const char* fmt, ...) {
    int old_size = buf.size();
    va_list args = {};
    va_start(args, fmt);
    buf.appendfv(fmt, args);
    va_end(args);
    for (int new_size = buf.size(); old_size < new_size; old_size++) {
        if (buf[old_size] == '\n') {
            line_offsets.push_back(old_size + 1);
        }
    }
}

void LogGui::draw(const char* title, bool* p_open) {
    if (!ImGui::Begin(title, p_open)) {
        ImGui::End();
        return;
    }

    // Options menu
    if (ImGui::BeginPopup("Options")) {
        ImGui::Checkbox("Auto-scroll", &auto_scroll);
        ImGui::EndPopup();
    }

    // Main window
    if (ImGui::Button("Options")) {
        ImGui::OpenPopup("Options");
    }
    ImGui::SameLine();
    bool clear_req = ImGui::Button("Clear");
    ImGui::SameLine();
    bool copy = ImGui::Button("Copy");
    ImGui::SameLine();
    filter.Draw("Filter", -100.0F);

    ImGui::Separator();
    ImGui::BeginChild("scrolling", ImVec2(0, 0), false,
                      ImGuiWindowFlags_HorizontalScrollbar);

    if (clear_req) {
        clear();
    }
    if (copy) {
        ImGui::LogToClipboard();
    }

    draw_text();

    if (auto_scroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
        ImGui::SetScrollHereY(1.0F);
    }

    ImGui::EndChild();
    ImGui::End();
}

void LogGui::draw_text() {
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
    const char* buf_start = buf.begin();
    const char* buf_end = buf.end();
    if (filter.IsActive()) {
        // In this example we don't use the clipper when Filter is enabled.
        // This is because we don't have a random access on the result on our
        // filter. A real application processing logs with ten of thousands of
        // entries may want to store the result of search/filter. especially if
        // the filtering function is not trivial (e.g. reg-exp).
        for (int line_no = 0; line_no < line_offsets.Size; line_no++) {
            // NOLINTBEGIN(cppcoreguidelines-pro-bounds-pointer-arithmetic)
            const char* line_start = buf_start + line_offsets[line_no];
            const char* line_end =
                (line_no + 1 < line_offsets.Size)
                    ? (buf_start + line_offsets[line_no + 1] - 1)
                    : buf_end;
            // NOLINTEND(cppcoreguidelines-pro-bounds-pointer-arithmetic)
            if (filter.PassFilter(line_start, line_end)) {
                ImGui::TextUnformatted(line_start, line_end);
            }
        }
    } else {
        // The simplest and easy way to display the entire buffer:
        //   ImGui::TextUnformatted(buf_begin, buf_end);
        // And it'll just work. TextUnformatted() has specialization for large
        // blob of text and will fast-forward to skip non-visible lines. Here we
        // instead demonstrate using the clipper to only process lines that are
        // within the visible area. If you have tens of thousands of items and
        // their processing cost is non-negligible, coarse clipping them on your
        // side is recommended. Using ImGuiListClipper requires A) random access
        // into your data, and B) items all being the  same height, both of
        // which we can handle since we an array pointing to the beginning of
        // each line of text. When using the filter (in the block of code above)
        // we don't have random access into the data to display anymore, which
        // is why we don't use the clipper. Storing or skimming through the
        // search result would make it possible (and would be recommended if you
        // want to search through tens of thousands of entries)
        ImGuiListClipper clipper;
        clipper.Begin(line_offsets.Size);
        while (clipper.Step()) {
            for (int line_no = clipper.DisplayStart;
                 line_no < clipper.DisplayEnd; line_no++) {
                // NOLINTBEGIN(cppcoreguidelines-pro-bounds-pointer-arithmetic)
                const char* line_start = buf_start + line_offsets[line_no];
                const char* line_end =
                    (line_no + 1 < line_offsets.Size)
                        ? (buf_start + line_offsets[line_no + 1] - 1)
                        : buf_end;
                // NOLINTEND(cppcoreguidelines-pro-bounds-pointer-arithmetic)
                ImGui::TextUnformatted(line_start, line_end);
            }
        }
        clipper.End();
    }
    ImGui::PopStyleVar();
}