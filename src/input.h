#pragma once

struct GLFWwindow;

// handle user inputs
namespace Input
{
void key_callback(
    GLFWwindow* window,
    int key,
    int scancode,
    int action,
    int mods
);
} // namespace Input
