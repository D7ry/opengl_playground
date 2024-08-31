#pragma once

// structs shared across calls

class TextureManager;
class Camera;
class InputManager;

struct InitData
{
    GLFWwindow* window;
    TextureManager* texture_manager;
    InputManager* input;
};

struct TickData
{
    Camera* camera;
    TextureManager* texture_manager;
    InputManager* input;
    double dt;      // delta time
    glm::mat4 proj; // projection matrix of main viewport
};

