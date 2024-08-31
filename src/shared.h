#pragma once

// structs shared across calls

class TextureManager;
class Camera;

struct InitData
{
    GLFWwindow* window;
    TextureManager* texture_manager;
};

struct TickData
{
    Camera* camera;
    TextureManager* texture_manager;
    double dt;      // delta time
    glm::mat4 proj; // projection matrix of main viewport
};

