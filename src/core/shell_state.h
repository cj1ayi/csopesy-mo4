#pragma once

// Every app the taskbar can launch
enum class AppId {
    Files,
    SystemInfo,
    TaskManager,
    Count
};

constexpr int APP_COUNT = static_cast<int>(AppId::Count);

struct ShellState {
    bool running = true;
    bool app_open[APP_COUNT] = {};   // all false at startup

    bool is_open(AppId id) const { return app_open[static_cast<int>(id)]; }
    void toggle(AppId id)        { app_open[static_cast<int>(id)] = !is_open(id); }
};
