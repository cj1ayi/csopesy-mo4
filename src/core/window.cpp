#include "window.h"
#include "config.h"

#include <GLFW/glfw3.h>
#include <cstdio>

bool Window::init(int width, int height, const char* title) {
    if (!glfwInit()) {
        std::fprintf(stderr, "Failed to initialize GLFW\n");
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GL_CONTEXT_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GL_CONTEXT_MINOR);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!m_window) {
        std::fprintf(stderr, "Failed to create window\n");
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1); // vsync
    return true;
}

void Window::shutdown() {
    if (m_window) {
        glfwDestroyWindow(m_window);
        m_window = nullptr;
    }
    glfwTerminate();
}

void Window::poll_events()  { glfwPollEvents(); }
void Window::swap_buffers() { glfwSwapBuffers(m_window); }

void Window::get_framebuffer_size(int& width, int& height) const {
    glfwGetFramebufferSize(m_window, &width, &height);
}