#pragma once

#include "core/shell_state.h"
#include "util/texture.h"

class Taskbar {
public:
    bool init();
    void shutdown();
    void render(ShellState& state);

private:
    void draw_app_buttons(ShellState& state);
    void draw_power_button(ShellState& state);

    Texture m_power_icon;
};
