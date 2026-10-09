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