#include "desktop.h"
#include "core/config.h"

#include "imgui.h"

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