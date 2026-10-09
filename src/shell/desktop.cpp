#include "desktop.h"
#include "core/config.h"

#include "imgui.h"

#include <ctime>
#include <cfloat>

bool Desktop::init() {
    m_wallpaper = load_texture(WALLPAPER_PATH);
    m_power_icon = load_texture(POWER_ICON_PATH);

    // fall back to solid color if missing wallpaper
    return true;
}

void Desktop::shutdown() {
    free_texture(m_wallpaper);
    free_texture(m_power_icon);
}

void Desktop::render(ShellState& state) {
    draw_wallpaper();
    draw_clock();
    draw_power_button(state);
}

void Desktop::draw_wallpaper() {
    ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
    const ImVec2 screen_size = ImGui::GetIO().DisplaySize;

    if (m_wallpaper.is_valid()) {
        draw_list->AddImage(
            (ImTextureID)(intptr_t)m_wallpaper.id,
            ImVec2(0.0f, 0.0f),
            screen_size);
    } else {
        // fallback: simple vertical gradient
        draw_list->AddRectFilledMultiColor(
            ImVec2(0.0f, 0.0f), screen_size,
            IM_COL32(20, 40, 90, 255), IM_COL32(20, 40, 90, 255),
            IM_COL32(5, 10, 30, 255),  IM_COL32(5, 10, 30, 255));
    }
}

std::string Desktop::get_clock_text() const {
    std::time_t now = std::time(nullptr);
    std::tm local_time{};

#ifdef _WIN32
    localtime_s(&local_time, &now);
#else
    localtime_r(&now, &local_time);
#endif

    char buffer[96];
    std::strftime(buffer, sizeof(buffer), CLOCK_FORMAT, &local_time);

    return std::string(buffer);
}

void Desktop::draw_clock() {
    ImDrawList*  draw_list   = ImGui::GetBackgroundDrawList();
    const ImVec2 screen_size = ImGui::GetIO().DisplaySize;

    const std::string text = get_clock_text();
    ImFont*           font = ImGui::GetFont();

    const ImVec2 text_size =
        font->CalcTextSizeA(CLOCK_FONT_SIZE, FLT_MAX, 0.0f, text.c_str());

    // Box anchored to the top-right corner
    const ImVec2 box_max(screen_size.x - CLOCK_MARGIN,
                         CLOCK_MARGIN + text_size.y + CLOCK_PADDING * 2.0f);
    const ImVec2 box_min(box_max.x - text_size.x - CLOCK_PADDING * 2.0f,
                         CLOCK_MARGIN);

    // Semi-transparent dark background so the text is readable on any wallpaper
    draw_list->AddRectFilled(box_min, box_max, IM_COL32(0, 0, 0, 170), 4.0f);

    const ImVec2 text_pos(box_min.x + CLOCK_PADDING, box_min.y + CLOCK_PADDING);
    draw_list->AddText(font, CLOCK_FONT_SIZE, text_pos,
                       IM_COL32(255, 255, 255, 255), text.c_str());
}

void Desktop::draw_power_button(ShellState& state) {
    const ImVec2 screen_size = ImGui::GetIO().DisplaySize;

    // Anchor the window's bottom-right corner to the screen's bottom-right corner
    ImGui::SetNextWindowPos(
        ImVec2(screen_size.x - POWER_BUTTON_MARGIN,
               screen_size.y - POWER_BUTTON_MARGIN),
        ImGuiCond_Always,
        ImVec2(1.0f, 1.0f));

    const ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoBackground |
        ImGuiWindowFlags_AlwaysAutoResize |
        ImGuiWindowFlags_NoFocusOnAppearing |
        ImGuiWindowFlags_NoBringToFrontOnFocus;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,
                        ImVec2(POWER_BUTTON_PADDING, POWER_BUTTON_PADDING));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding,
                        POWER_ICON_SIZE * 0.5f + POWER_BUTTON_PADDING); // fully round

    ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4(0.0f, 0.0f, 0.0f, 0.65f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.80f, 0.10f, 0.10f, 0.90f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4(0.60f, 0.05f, 0.05f, 1.00f));

    ImGui::Begin("##power_button", nullptr, flags);

    bool clicked = false;
    if (m_power_icon.is_valid()) {
        clicked = ImGui::ImageButton(
            "##power_icon",
            (ImTextureID)(intptr_t)m_power_icon.id,
            ImVec2(POWER_ICON_SIZE, POWER_ICON_SIZE));
    } else {
        // Fallback if the PNG didn't load
        clicked = ImGui::Button("PWR");
    }

    // Button hover feedback
    if (ImGui::IsItemHovered())
        ImGui::SetTooltip("Shut down");

    ImGui::End();

    ImGui::PopStyleColor(3);
    ImGui::PopStyleVar(4);

    if (clicked)
        state.running = false;
}