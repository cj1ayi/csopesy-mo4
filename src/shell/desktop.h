#pragma once

#include <string>
#include "util/texture.h"

class Desktop {
public:
    bool init();
    void shutdown();
    void render();

private:
    void draw_wallpaper();
    void draw_clock();

    std::string get_clock_text() const;

    Texture m_wallpaper;
};