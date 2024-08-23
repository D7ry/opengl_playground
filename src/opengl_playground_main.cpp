#include "stdio.h"
#include <iostream>
// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// clang-format onen
#include "input.h"

/* Constants */
const size_t WINDOW_WIDTH = 800;
const size_t WINDOW_HEIGHT = 800;
const size_t VIEWPORT_WIDTH = 800;
const size_t VIEWPORT_HEIGHT = 800;


// main render loop
void render_loop(GLFWwindow* window) {
    printf("Entering render loop...\n");
    fflush(stdout);
    while (!glfwWindowShouldClose(window)) {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    printf("Exiting render loop...\n");
}


namespace Callback
{
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
} // namespace Callback

int main() {
    GLFWwindow* window = nullptr;
    { // initialize glfw
        printf("Initializing GLFW... \n");
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
        printf("... success\n");
    }
    { // initialize openGL
        // glad
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cout << "Failed to initialize GLAD" << std::endl;
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
        glfwSetFramebufferSizeCallback(window, Callback::framebuffer_size_callback);
    }

    // main render loop
    glfwSetKeyCallback(window, Input::key_callback);
    render_loop(window);

    return 0;
}
