#pragma once

#include "window.h"
#include "shell_state.h"
#include "shell/desktop.h"
#include "shell/taskbar.h"

class App {
public:
    bool init();
    void run();
    void shutdown();

private:
    void begin_frame();
    void render_shell();   // desktop, apps, taskbar will be drawn here
    void end_frame();

    Desktop     m_desktop;
    Taskbar     m_taskbar;
    Window      m_window;
    ShellState  m_state;
};