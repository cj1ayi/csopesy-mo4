#pragma once

#include <cstdint>

struct Texture {
    uint32_t    id      = 0;
    int         width   = 0;
    int         height  = 0;

    bool is_valid() const { return id != 0; }
};

Texture load_texture(const char *path);
void free_texture(Texture& texture);