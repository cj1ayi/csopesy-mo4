#include "taskbar.h"
#include "core/config.h"

#include "imgui.h"
#include "imgui_internal.h"   // for BringWindowToDisplayFront

namespace {

// one entry per app button 
struct TaskbarButton {
    AppId       app;
    const char* label;     // short text drawn inside the round button
    const char* tooltip;   // shown on hover
};

constexpr TaskbarButton TASKBAR_BUTTONS[] = {
    { AppId::Files,       "FILE", "File Explorer" },
    { AppId::SystemInfo,  "INFO", "System Info"   },
    { AppId::TaskManager, "TASK", "Task Manager"  },
};

const ImVec4 BUTTON_IDLE    (1.00f, 1.00f, 1.00f, 0.08f);
const ImVec4 BUTTON_OPEN    (0.25f, 0.45f, 0.85f, 0.55f);
const ImVec4 BUTTON_HOVERED (1.00f, 1.00f, 1.00f, 0.20f);
const ImVec4 BUTTON_ACTIVE  (1.00f, 1.00f, 1.00f, 0.30f);
const ImU32  RUNNING_BAR_COLOR = IM_COL32(120, 170, 255, 255);

} // namespace

bool Taskbar::init() {
    m_power_icon = load_texture(POWER_ICON_PATH);
    return true;   // fallback to text incase of missing icon
}

void Taskbar::shutdown() {
    free_texture(m_power_icon);
}

void Taskbar::render(ShellState& state) {
    const ImVec2 screen_size = ImGui::GetIO().DisplaySize;

    // stretch across the full width, glued to the bottom edge
    ImGui::SetNextWindowPos(ImVec2(0.0f, screen_size.y - TASKBAR_HEIGHT));
    ImGui::SetNextWindowSize(ImVec2(screen_size.x, TASKBAR_HEIGHT));

    const ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoFocusOnAppearing;

    // centers the buttons 
    const float padding_y = (TASKBAR_HEIGHT - TASKBAR_BUTTON_SIZE) * 0.5f;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(TASKBAR_PADDING_X, padding_y));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, TASKBAR_BUTTON_SIZE * 0.5f); 
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.05f, 0.05f, 0.08f, 0.92f));

    ImGui::Begin("##taskbar", nullptr, flags);


    ImGui::BringWindowToDisplayFront(ImGui::GetCurrentWindow());

    draw_app_buttons(state);
    draw_power_button(state);

    ImGui::End();

    ImGui::PopStyleColor(1);
    ImGui::PopStyleVar(4);
}

void Taskbar::draw_app_buttons(ShellState& state) {
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    for (int i = 0; i < IM_ARRAYSIZE(TASKBAR_BUTTONS); ++i) {
        const TaskbarButton& button = TASKBAR_BUTTONS[i];
        const bool open = state.is_open(button.app);

        if (i > 0)
            ImGui::SameLine(0.0f, TASKBAR_BUTTON_SPACING);

        ImGui::PushStyleColor(ImGuiCol_Button,        open ? BUTTON_OPEN : BUTTON_IDLE);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, BUTTON_HOVERED);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive,  BUTTON_ACTIVE);

        if (ImGui::Button(button.label, ImVec2(TASKBAR_BUTTON_SIZE, TASKBAR_BUTTON_SIZE)))
            state.toggle(button.app);

        ImGui::PopStyleColor(3);

        if (ImGui::IsItemHovered())
            ImGui::SetTooltip("%s", button.tooltip);

        // "Running" indicator: a small bar under the button, like windows
        if (open) {
            const ImVec2 item_min = ImGui::GetItemRectMin();
            const ImVec2 item_max = ImGui::GetItemRectMax();
            const float  center_x = (item_min.x + item_max.x) * 0.5f;

            draw_list->AddRectFilled(
                ImVec2(center_x - 8.0f, item_max.y + 3.0f),
                ImVec2(center_x + 8.0f, item_max.y + 6.0f),
                RUNNING_BAR_COLOR, 2.0f);
        }
    }
}

void Taskbar::draw_power_button(ShellState& state) {
    // Jump to the right edge of the bar, on the same row as the app buttons
    const float x = ImGui::GetWindowWidth() - TASKBAR_PADDING_X - TASKBAR_BUTTON_SIZE;
    ImGui::SameLine(x);

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,
                        ImVec2(POWER_BUTTON_PADDING, POWER_BUTTON_PADDING));
    ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.80f, 0.10f, 0.10f, 0.90f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4(0.60f, 0.05f, 0.05f, 1.00f));

    bool clicked = false;
    if (m_power_icon.is_valid()) {
        clicked = ImGui::ImageButton(
            "##power_icon",
            (ImTextureID)(intptr_t)m_power_icon.id,
            ImVec2(POWER_ICON_SIZE, POWER_ICON_SIZE));
    } else {
        // fallback if the PNG didn't load
        clicked = ImGui::Button("PWR", ImVec2(TASKBAR_BUTTON_SIZE, TASKBAR_BUTTON_SIZE));
    }

    ImGui::PopStyleColor(3);
    ImGui::PopStyleVar(1);

    if (ImGui::IsItemHovered())
        ImGui::SetTooltip("Shut down");

    if (clicked)
        state.running = false;
}
