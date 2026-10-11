#include "texture.h"

#include <GLFW/glfw3.h>
#include <cstdio>

#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE 0x812F
#endif

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture load_texture(const char *path) {
    Texture texture;

    unsigned char* pixels =
        stbi_load(path, &texture.width, &texture.height, nullptr, 4); // force RGBA
    if (!pixels) {
        std::fprintf(stderr, "Failed to load image: %s\n", path);
        return texture;
    }

    GLuint gl_id = 0;
    glGenTextures(1, &gl_id);
    glBindTexture(GL_TEXTURE_2D, gl_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width, texture.height,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    stbi_image_free(pixels);
    texture.id = gl_id;
    return texture;
}

void free_texture(Texture& texture) {
    if (texture.id != 0) {
        GLuint gl_id = texture.id;
        glDeleteTextures(1, &gl_id);
        texture.id = 0;
    }
}