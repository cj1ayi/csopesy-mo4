#include "system_info.h"
#include "core/config.h"

#include "imgui.h"


namespace {

struct SummaryItem {
    const char* item;
    const char* value;
};

constexpr SummaryItem SYSTEM_SUMMARY[] = {
    { "OS Name",                         "Windows 67" },
    { "Version",                         "1.0.0 Build 4004" },
    { "Other OS Description",            "Not Available" },
    { "OS Manufacturer",                 "Microslop" },
    { "System Name",                     "CSOPESY-PC" },
    { "System Manufacturer",             "CSOPESY Group" },
    { "System Model",                    "Virtual Desktop" },
    { "System Type",                     "x64-based PC" },
    { "System SKU",                      "MO4" },
    { "Processor",                       "Virtual CPU @ 6.00GHz, 64 Core(s), 128 Logical Processor(s)" },
    { "BIOS Version/Date",               "CSOPESY BIOS 1.04, 10/11/2026" },
    { "SMBIOS Version",                  "3.4" },
    { "Embedded Controller Version",     "255.255" },
    { "BIOS Mode",                       "UEFI" },
    { "BaseBoard Manufacturer",          "CSOPESY Group" },
    { "BaseBoard Product",               "MO4 Board" },
    { "BaseBoard Version",               "1.0" },
    { "Platform Role",                   "Desktop" },
    { "Secure Boot State",               "On" },
    { "PCR7 Configuration",              "Elevation Required to View" },
    { "Boot Device",                     "\\Device\\HarddiskVolume1" },
    { "Locale",                          "Philippines" },
    { "Hardware Abstraction Layer",      "Version = \"1.0.0\"" },
    { "User Name",                       "CSOPESY-PC\\user" },
    { "Time Zone",                       "Singapore Standard Time" },
    { "Installed Physical Memory (RAM)", "768.00 GB" },
    { "Total Physical Memory",           "767.84 GB" },
    { "Available Physical Memory",       "764.21 GB" },
    { "Total Virtual Memory",            "9.09 GB" },
    { "Available Virtual Memory",        "5.12 GB" },
    { "Page File Space",                 "1.25 GB" },
    { "Page File",                       "C:\\pagefile.sys" },
    { "Kernel DMA Protection",           "Off" },
    { "Virtualization-based security",   "Not enabled" },
    { "Device Encryption Support",       "Elevation Required to View" },
    { "Hyper-V - VM Monitor Mode Extensions",          "Yes" },
    { "Hyper-V - Second Level Address Translation Extensions", "Yes" },
    { "Hyper-V - Virtualization Enabled in Firmware",  "Yes" },
    { "Hyper-V - Data Execution Protection",           "Yes" },
};

}

bool SystemInfo::init() { return true; }

void SystemInfo::shutdown() {}

void SystemInfo::render(ShellState& state) {
    if (!state.is_open(AppId::SystemInfo))
        return;

    const ImVec2 screen_size = ImGui::GetIO().DisplaySize;
    ImGui::SetNextWindowPos(ImVec2(screen_size.x * 0.5f, (screen_size.y - TASKBAR_HEIGHT) * 0.5f),
                            ImGuiCond_FirstUseEver, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(SYSINFO_WIDTH, SYSINFO_HEIGHT), ImGuiCond_FirstUseEver);

    bool open = true;
    if (ImGui::Begin("System Information", &open, ImGuiWindowFlags_NoCollapse))
        draw_summary_table();
    ImGui::End();

    // window's X button closes the app, same as clicking INFO on the taskbar
    if (!open)
        state.toggle(AppId::SystemInfo);
}

void SystemInfo::draw_summary_table() {
    const ImGuiTableFlags table_flags =
        ImGuiTableFlags_Borders |
        ImGuiTableFlags_RowBg |
        ImGuiTableFlags_Resizable |
        ImGuiTableFlags_ScrollY;

    if (!ImGui::BeginTable("##summary", 2, table_flags))
        return;

    ImGui::TableSetupScrollFreeze(0, 1);   // keep the header visible while scrolling
    ImGui::TableSetupColumn("Item",  ImGuiTableColumnFlags_WidthFixed, 260.0f);
    ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);
    ImGui::TableHeadersRow();

    for (const SummaryItem& row : SYSTEM_SUMMARY) {
        ImGui::TableNextRow();
        ImGui::TableNextColumn(); ImGui::TextUnformatted(row.item);
        ImGui::TableNextColumn(); ImGui::TextUnformatted(row.value);
    }

    ImGui::EndTable();
}
