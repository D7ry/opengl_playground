#include "stdio.h"
#include "input.h"

#include <GLFW/glfw3.h>

void Input::key_callback(
    GLFWwindow* window,
    int key,
    int scancode,
    int action,
    int mods
) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        printf("Exiting window\n");
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}
