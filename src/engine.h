#pragma once

#include "camera.h"
#include "delta_time.h"
#include "input.h"
#include "shader.h"

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

    struct DemoData
    {
        unsigned int vao;
        SimpleShaderProgram* shaders = nullptr;
    } demo_data;

    void bind_default_inputs();

    void demo_init();

    void demo_tick();

    void toggle_imgui_control(bool on);
};
