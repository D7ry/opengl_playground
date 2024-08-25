#include "input.h"
#include "lab.h"
#include "stdio.h"
#include <iostream>

// global engine states,
// TODO: need to migrate to a proper class soon
namespace EngineState
{
// whether glfw window should capture the cursor
bool window_capture_cursor = false;
}

void global_key_callback(
    GLFWwindow* window,
    int key,
    int scancode,
    int action,
    int mods
) {
    InputManager::get_singleton()->on_key_input(
        window, key, scancode, action, mods
    );
    ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
}

// main render loop
void render_loop(GLFWwindow* window) {
    INFO("Render loop started");
    fflush(stdout);
    Lab::HelloTriangle::init(window);
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwPollEvents();
        { // imgui prologue
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
        }

        Lab::HelloTriangle::tick();

        { // imgui epilogue
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }

        glfwSwapBuffers(window);
    }
    INFO("Render loop ended");
}

namespace Callback
{
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
} // namespace Callback

int main() {
    Logging::init();
    GLFWwindow* window = nullptr;
    { // initialize glfw
        INFO("Initializing GLFW...");
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        window = glfwCreateWindow(
            WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGLPlayground", NULL, NULL
        );
        if (!window) {
            printf("Failed to create GLFW window\n");
            glfwTerminate();
            return -1;
        }
        glfwMakeContextCurrent(window);
        INFO("...success");
    }
    { // initialize openGL
        // glad
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            ERROR("Failed to initialize GLAD");
            return -1;
        }
        // set viewport to occupy the whole screen. Currently we only need one
        // viewport
        glViewport(
            0,              // x
            0,              // y
            VIEWPORT_WIDTH, // width
            VIEWPORT_HEIGHT // height
        );
        glfwSetFramebufferSizeCallback(
            window, Callback::framebuffer_size_callback
        );
    }

    { // initialize imgui
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags
            |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        io.ConfigFlags
            |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls
        // disable input capture
        io.WantCaptureMouse = false;
        io.WantCaptureKeyboard = false;
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init();

        // key to flip imgui input capture
        const int IMGUI_INPUT_CAPTURE_KEY = GLFW_KEY_TAB;
        InputManager::get_singleton()->register_key_callback(
            IMGUI_INPUT_CAPTURE_KEY,
            InputManager::KeyCallbackCondition::PRESS,
            [window]() {
                DEBUG("Toggle cursor");
                auto CURSOR_FLAG = EngineState::window_capture_cursor
                                       ? GLFW_CURSOR_DISABLED
                                       : GLFW_CURSOR_NORMAL;
                EngineState::window_capture_cursor
                    = !EngineState::window_capture_cursor;
                glfwSetInputMode(window, GLFW_CURSOR, CURSOR_FLAG);
            }
        );
    }

    // main render loop
    { // set up InputManager
        glfwSetKeyCallback(window, global_key_callback);
    }
    render_loop(window);

    return 0;
}
