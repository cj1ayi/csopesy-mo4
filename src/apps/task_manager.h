#pragma once

#include "core/shell_state.h"

// Task Manager listing the open apps as processes, with dummy usage numbers
class TaskManager {
public:
    bool init();
    void shutdown();
    void render(ShellState& state);

private:
    void draw_process_table(const ShellState& state);
};
