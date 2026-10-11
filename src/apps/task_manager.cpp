#include "task_manager.h"
#include "core/config.h"

#include "imgui.h"

#include <cmath>
#include <random>


namespace {

struct DummyProcess {
    const char* image;
    int         pid;
    int         base_cpu;      // percent, the value fluctuations swing around
    int         base_mem_kb;
    int         cpu    = 0;    // values currently shown, set from the base in init()
    int         mem_kb = 0;
};

// "processes" that are always running
DummyProcess SYSTEM_PROCESSES[] = {
    { "compositor.exe",       412, 3, 18432 },
    { "WallpaperService.exe", 436, 0,  9216 },
    { "ClockService.exe",     452, 0,  1024 },
};

// dummy usage numbers for each of our defined apps
DummyProcess APP_PROCESSES[APP_COUNT] = {
    { "explorer.exe", 1548, 2, 21344 },   // AppId::Files
    { "sysinfo.exe",  2208, 0,  5612 },   // AppId::SystemInfo
    { "taskmgr.exe",  2520, 1,  4296 },   // AppId::TaskManager
};

std::mt19937 rng{ std::random_device{}() };

// randomly choose to fluctuate the cpu and mem
void maybe_fluctuate(DummyProcess& process) {
    std::uniform_real_distribution<float> roll(0.0f, 1.0f);
    if (roll(rng) >= TASKMGR_RAND_CHANCE)
        return;

    std::uniform_real_distribution<float> swing(-TASKMGR_RAND_VOLATILITY, TASKMGR_RAND_VOLATILITY);
    process.cpu    = std::max(0, process.base_cpu + static_cast<int>(std::lround(swing(rng) * 100.0f)));
    process.mem_kb = static_cast<int>(std::lround(process.base_mem_kb * (1.0f + swing(rng))));
}

void draw_process_row(DummyProcess& process) {
    maybe_fluctuate(process);

    ImGui::TableNextRow();
    ImGui::TableNextColumn(); ImGui::TextUnformatted(process.image);
    ImGui::TableNextColumn(); ImGui::Text("%d", process.pid);
    ImGui::TableNextColumn(); ImGui::Text("%02d", process.cpu);
    ImGui::TableNextColumn(); ImGui::Text("%d K", process.mem_kb);
}

}

bool TaskManager::init() {
    for (DummyProcess& process : SYSTEM_PROCESSES) {
        process.cpu    = process.base_cpu;
        process.mem_kb = process.base_mem_kb;
    }
    for (DummyProcess& process : APP_PROCESSES) {
        process.cpu    = process.base_cpu;
        process.mem_kb = process.base_mem_kb;
    }
    return true;
}

void TaskManager::shutdown() {}

void TaskManager::render(ShellState& state) {
    if (!state.is_open(AppId::TaskManager))
        return;

    const ImVec2 screen_size = ImGui::GetIO().DisplaySize;
    ImGui::SetNextWindowPos(ImVec2(screen_size.x * 0.5f, (screen_size.y - TASKBAR_HEIGHT) * 0.5f),
                            ImGuiCond_FirstUseEver, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(TASKMGR_WIDTH, TASKMGR_HEIGHT), ImGuiCond_FirstUseEver);

    bool open = true;
    if (ImGui::Begin("Task Manager", &open, ImGuiWindowFlags_NoCollapse))
        draw_process_table(state);
    ImGui::End();

    // window's X button closes the app, same as clicking TASK on the taskbar
    if (!open)
        state.toggle(AppId::TaskManager);
}

void TaskManager::draw_process_table(const ShellState& state) {
    const ImGuiTableFlags table_flags =
        ImGuiTableFlags_Borders |
        ImGuiTableFlags_RowBg |
        ImGuiTableFlags_Resizable |
        ImGuiTableFlags_ScrollY;

    if (!ImGui::BeginTable("##processes", 4, table_flags))
        return;

    ImGui::TableSetupScrollFreeze(0, 1);   // keep the header visible while scrolling
    ImGui::TableSetupColumn("Process", ImGuiTableColumnFlags_WidthStretch);
    ImGui::TableSetupColumn("PID",        ImGuiTableColumnFlags_WidthFixed, 40.0f);
    ImGui::TableSetupColumn("CPU (%)",        ImGuiTableColumnFlags_WidthFixed, 50.0f);
    ImGui::TableSetupColumn("Mem Usage",  ImGuiTableColumnFlags_WidthFixed, 75.0f);
    ImGui::TableHeadersRow();

    for (DummyProcess& process : SYSTEM_PROCESSES) {
        draw_process_row(process);
    }

    // plus one row per app currently open on the taskbar
    for (int i = 0; i < APP_COUNT; ++i)
        if (state.app_open[i])
            draw_process_row(APP_PROCESSES[i]);

    ImGui::EndTable();
}
