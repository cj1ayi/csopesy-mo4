#pragma once

#include <string>
#include "core/shell_state.h"
#include "util/texture.h"

class Desktop {
public:
    bool init();
    void shutdown();
    void render(ShellState& state);

private:
    void draw_wallpaper();
    void draw_clock();
    void draw_power_button(ShellState& state);

    std::string get_clock_text() const;

    Texture m_wallpaper;
    Texture m_power_icon;
};