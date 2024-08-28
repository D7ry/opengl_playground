#pragma once

#include "camera.h"
#include "delta_time.h"
#include "input.h"
#include "shader.h"
#include "mesh.h"
#include "app/app.h"

// a self-contained engine. Should be used as a singleton for each thread.
class Engine
{
  public:
    static Engine* get_singleton();

    void render_loop();

    Engine() = delete;

    ~Engine() {}

  private:
    GLFWwindow* window;
    std::unique_ptr<InputManager> input;
    std::unique_ptr<Camera> camera;

    DeltaTimer delta_time;
    bool imgui_control;
    bool cursor_capture;

    static void glfw_key_callback(
        GLFWwindow* window,
        int key,
        int scancode,
        int action,
        int mods
    );

    static void glfw_cursor_pos_callback(
        GLFWwindow* window,
        double xpos,
        double ypos
    );

    Engine(const std::string& window_name);

    void bind_default_inputs();

    void draw_debug_window() {
        ImGui::Begin("Debug");
        {
            ImGui::Text("Performance");
            ImGui::Text("FPS: %f", (1.f / delta_time.get()));
        }
        ImGui::Separator();
        this->camera->debug_info_imgui();
        ImGui::End();
    }

    // input handlers
    void toggle_imgui_control();
    void toggle_cursor_capture();

    std::vector<std::unique_ptr<App>> apps;
};
