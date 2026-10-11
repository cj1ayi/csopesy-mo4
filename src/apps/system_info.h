#pragma once

#include "core/shell_state.h"

class SystemInfo {
public:
    bool init();
    void shutdown();
    void render(ShellState& state);

private:
    void draw_summary_table();
};
