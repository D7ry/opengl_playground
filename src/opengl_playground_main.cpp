#include "stdio.h"
#include <iostream>
// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>
// clang-format on
#include "input.h"
#include "lab.h"

/* Constants */
const size_t WINDOW_WIDTH = 800;
const size_t WINDOW_HEIGHT = 800;
const size_t VIEWPORT_WIDTH = 800;
const size_t VIEWPORT_HEIGHT = 800;

// main render loop
void render_loop(GLFWwindow* window) {
    INFO("Render loop started");
    fflush(stdout);
    Lab::HelloTriangle::init(window);
    while (!glfwWindowShouldClose(window)) {
        glfwSwapBuffers(window);
        glfwPollEvents();
        Lab::HelloTriangle::tick();
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
    spdlog::set_pattern("\033[1;37m[%^%l%$] [%s:%#] (%!) %v\033[0m");
    GLFWwindow* window = nullptr;
    { // initialize glfw
        INFO("Initializing GLFW...");
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        window = glfwCreateWindow(
            WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGLPlayground", NULL, NULL
        );
        // disable cursor by default
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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

    // main render loop
    glfwSetKeyCallback(window, Input::key_callback);
    render_loop(window);

    return 0;
}
