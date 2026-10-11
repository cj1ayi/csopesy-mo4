#pragma once

struct GLFWwindow;

class Window {
public:
    bool init(int width, int height, const char* title);
    void shutdown();

    void poll_events();
    void swap_buffers();
    void get_framebuffer_size(int& width, int& height) const;

    GLFWwindow* handle() const { return m_window; }

private:
    GLFWwindow* m_window = nullptr;
};