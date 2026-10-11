#pragma once

#include "core/shell_state.h"

// File Explorer displaying a static folder sidebar and mock text files.
class FileExplorer {
public:
    bool init();
    void shutdown();
    void render(ShellState& state);

private:
    void draw_contents();
    void draw_sidebar(float height);
    void draw_file_table(float height);
};
