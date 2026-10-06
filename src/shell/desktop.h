#pragma once

#include "util/texture.h"

class Desktop {
public:
    bool init();
    void shutdown();
    void render();

private:
    void draw_wallpaper();

    Texture m_wallpaper;
};