#pragma once

// Application window
constexpr int         WINDOW_WIDTH      = 1280;
constexpr int         WINDOW_HEIGHT     = 720;
constexpr const char* WINDOW_TITLE      = "CSOPESY MO4 - Desktop GUI";
constexpr const char* GLSL_VERSION      = "#version 330";
constexpr int         GL_CONTEXT_MAJOR  = 3;
constexpr int         GL_CONTEXT_MINOR  = 3;

// Wallpaper
constexpr const char* WALLPAPER_PATH    = "assets/wallpaper.jpg";

// Clock
constexpr const char* CLOCK_FORMAT    = "%A, %b %d, %Y | %I:%M:%S %p";
constexpr float       CLOCK_FONT_SIZE = 20.0f;
constexpr float       CLOCK_MARGIN    = 12.0f;   // gap from the screen edges
constexpr float       CLOCK_PADDING   = 8.0f;    // space between text and box edge

// Taskbar
constexpr float       TASKBAR_HEIGHT         = 64.0f;
constexpr float       TASKBAR_BUTTON_SIZE    = 44.0f;   // diameter of each round button
constexpr float       TASKBAR_BUTTON_SPACING = 12.0f;   // gap between buttons
constexpr float       TASKBAR_PADDING_X      = 16.0f;   // gap from the left/right screen edges

// Power button (lives on the right side of the taskbar)
constexpr const char* POWER_ICON_PATH      = "assets/icons/power-button.png";
constexpr float       POWER_ICON_SIZE      = 28.0f;   // icon image size in pixels
constexpr float       POWER_BUTTON_PADDING =          // space around the icon so the button
    (TASKBAR_BUTTON_SIZE - POWER_ICON_SIZE) * 0.5f;   // matches the other taskbar buttons

// File Explorer
constexpr float       FILES_WIDTH         = 720.0f;
constexpr float       FILES_HEIGHT        = 460.0f;
constexpr float       FILES_MIN_WIDTH     = 560.0f;
constexpr float       FILES_MIN_HEIGHT    = 320.0f;
constexpr float       FILES_SIDEBAR_WIDTH = 140.0f;

// System Information
constexpr float       SYSINFO_WIDTH  = 640.0f;
constexpr float       SYSINFO_HEIGHT = 480.0f;

// Task Manager
constexpr float       TASKMGR_WIDTH           = 460.0f;
constexpr float       TASKMGR_HEIGHT          = 420.0f;
constexpr float       TASKMGR_RAND_VOLATILITY = 0.05f; // cpu/mem usage will fluctuate by this much each frame
constexpr float       TASKMGR_RAND_CHANCE     = 0.01f; // chance per render that a row's values fluctuate
