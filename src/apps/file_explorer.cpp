#include "file_explorer.h"
#include "core/config.h"

#include "imgui.h"

#include <algorithm>
#include <cfloat>

namespace {

struct MockFile {
    const char* name;
    const char* modified;
    const char* type;
    const char* size;
};

constexpr MockFile DOCUMENTS[] = {
    { "readme.txt", "10/11/2026", "Text Document", "1 KB" },
    { "notes.txt",  "10/11/2026", "Text Document", "2 KB" },
};

constexpr const char* SIDEBAR_LOCATIONS[] = {
    "This PC", "Desktop", "Documents", "Downloads",
};
constexpr int DOCUMENTS_LOCATION = 2;

} // namespace

bool FileExplorer::init() { return true; }

void FileExplorer::shutdown() {}

void FileExplorer::render(ShellState& state) {
    if (!state.is_open(AppId::Files))
        return;

    const ImVec2 screen_size = ImGui::GetIO().DisplaySize;
    ImGui::SetNextWindowPos(ImVec2(screen_size.x * 0.5f, (screen_size.y - TASKBAR_HEIGHT) * 0.5f),
                            ImGuiCond_FirstUseEver, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(FILES_WIDTH, FILES_HEIGHT), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSizeConstraints(ImVec2(FILES_MIN_WIDTH, FILES_MIN_HEIGHT),
                                        ImVec2(FLT_MAX, FLT_MAX));

    bool open = true;
    if (ImGui::Begin("File Explorer", &open, ImGuiWindowFlags_NoCollapse))
        draw_contents();
    ImGui::End();

    // Match the FILE taskbar button when the window's X button is clicked.
    if (!open)
        state.toggle(AppId::Files);
}

void FileExplorer::draw_contents() {
    if (ImGui::BeginChild("##path", ImVec2(0.0f, ImGui::GetFrameHeight()),
                          ImGuiChildFlags_FrameStyle, ImGuiWindowFlags_NoScrollbar))
        ImGui::TextUnformatted("This PC > Documents");
    ImGui::EndChild();

    // Leave space below the sidebar and table for the separator and item count.
    const float status_height = ImGui::GetTextLineHeightWithSpacing()
                                + ImGui::GetStyle().ItemSpacing.y + 1.0f;
    const float content_height = std::max(1.0f, ImGui::GetContentRegionAvail().y - status_height);
    draw_sidebar(content_height);
    ImGui::SameLine();
    draw_file_table(content_height);

    ImGui::Separator();
    ImGui::Text("%d items", IM_ARRAYSIZE(DOCUMENTS));
}

void FileExplorer::draw_sidebar(float height) {
    if (ImGui::BeginChild("##locations", ImVec2(FILES_SIDEBAR_WIDTH, height),
                          ImGuiChildFlags_Borders)) {
        const ImGuiStyle& style = ImGui::GetStyle();
        const float row_height = ImGui::GetFrameHeight();

        for (int i = 0; i < IM_ARRAYSIZE(SIDEBAR_LOCATIONS); ++i) {
            const ImVec2 row_min = ImGui::GetCursorScreenPos();
            const float row_width = ImGui::GetContentRegionAvail().x;
            ImDrawList* draw_list = ImGui::GetWindowDrawList();

            // Decorative selection: folders do not navigate or react to clicks.
            if (i == DOCUMENTS_LOCATION) {
                draw_list->AddRectFilled(
                    row_min,
                    ImVec2(row_min.x + row_width, row_min.y + row_height),
                    ImGui::GetColorU32(ImGuiCol_Header), style.FrameRounding);
            }

            draw_list->AddText(ImVec2(row_min.x + style.FramePadding.x,
                                     row_min.y + style.FramePadding.y),
                               ImGui::GetColorU32(ImGuiCol_Text), SIDEBAR_LOCATIONS[i]);
            ImGui::Dummy(ImVec2(row_width, row_height));
        }
    }
    ImGui::EndChild();
}

void FileExplorer::draw_file_table(float height) {
    const ImGuiTableFlags table_flags =
        ImGuiTableFlags_Borders |
        ImGuiTableFlags_RowBg |
        ImGuiTableFlags_Resizable |
        ImGuiTableFlags_ScrollY;

    if (!ImGui::BeginTable("##files", 4, table_flags, ImVec2(0.0f, height)))
        return;

    ImGui::TableSetupScrollFreeze(0, 1);
    ImGui::TableSetupColumn("Name",          ImGuiTableColumnFlags_WidthStretch);
    ImGui::TableSetupColumn("Date modified", ImGuiTableColumnFlags_WidthFixed, 110.0f);
    ImGui::TableSetupColumn("Type",          ImGuiTableColumnFlags_WidthFixed, 100.0f);
    ImGui::TableSetupColumn("Size",          ImGuiTableColumnFlags_WidthFixed, 45.0f);
    ImGui::TableHeadersRow();

    for (const MockFile& file : DOCUMENTS) {
        ImGui::TableNextRow();
        ImGui::TableNextColumn(); ImGui::TextUnformatted(file.name);
        ImGui::TableNextColumn(); ImGui::TextUnformatted(file.modified);
        ImGui::TableNextColumn(); ImGui::TextUnformatted(file.type);
        ImGui::TableNextColumn(); ImGui::TextUnformatted(file.size);
    }

    ImGui::EndTable();
}
