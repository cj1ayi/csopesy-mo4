#include "app.h"
#include "config.h"

#include <GLFW/glfw3.h>
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

bool App::init() {
    if (!m_window.init(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE))
        return false;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::GetIO().IniFilename = nullptr; // don't save window layout between runs

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(m_window.handle(), true);
    ImGui_ImplOpenGL3_Init(GLSL_VERSION);

    if (!m_desktop.init())
        return false;

    if (!m_taskbar.init())
        return false;

    if (!m_system_info.init())
        return false;

    return true;
}

void App::run() {
    while (m_state.running) {
        m_window.poll_events();

        // Ignore the window's X button... only PWR button should close the app
        glfwSetWindowShouldClose(m_window.handle(), GLFW_FALSE);

        begin_frame();
        render_shell();
        end_frame();
    }
}

void App::begin_frame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void App::render_shell() {
    // Draw order = stacking order: back to front
    m_desktop.render();
    // TODO: apps (File Explorer, Task Manager)
    m_system_info.render(m_state);
    m_taskbar.render(m_state);   // last, so it sits on top
}

void App::end_frame() {
    ImGui::Render();

    int width, height;
    m_window.get_framebuffer_size(width, height);
    glViewport(0, 0, width, height);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    m_window.swap_buffers();
}

void App::shutdown() {
    // Reverse order of init
    m_system_info.shutdown();
    m_taskbar.shutdown();
    m_desktop.shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    m_window.shutdown();
}