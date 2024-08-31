#pragma once

#include "app/app.h"
#include "camera.h"
#include "delta_time.h"
#include "input.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"

// a self-contained engine. Should be used as a singleton for each thread.
class Engine
{
  public:
    static Engine* get_singleton();

    void init();

    void render_loop();

    Engine(const std::string& window_name);

    Engine() = delete;

    ~Engine() {}

    // register an app to the engine. Note that 
    // app should be registered before calling Engine::init()
    // for app-specific init() functions to work.
    void register_app(std::unique_ptr<App> app);

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

    static void glfw_fb_resize_callback(
        GLFWwindow* window,
        int width,
        int height
    );

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
    TextureManager texture_manager;
    std::string window_name;
};
