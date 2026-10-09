#include "desktop.h"
#include "core/config.h"

#include "imgui.h"

#include <ctime>
#include <cfloat>

bool Desktop::init() {
    m_wallpaper = load_texture(WALLPAPER_PATH);
    // fall back to solid color if missing wallpaper
    return true;
}

void Desktop::shutdown() {
    free_texture(m_wallpaper);
}

void Desktop::render() {
    draw_wallpaper();
    draw_clock();
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