#pragma once

struct Transform
{
    glm::vec3 position;
    glm::vec3 rotation; // yaw pitch roll
    glm::vec3 scale;    // x y z
    
    // get identity transform
    static Transform identity() {
        return Transform{glm::vec3(0.f), glm::vec3(0.f), glm::vec3(1.f)};
    }
};
