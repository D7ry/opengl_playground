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

    // get model matrix of the transform
    glm::mat4 model() {
        glm::mat4 model = glm::mat4(1.0f);
        // model = glm::translate(model, position);
        // model = glm::rotate(
        //     model, glm::radians(rotation.x), glm::vec3(1.f, 0.f, 0.f)
        // );
        // model = glm::rotate(
        //     model, glm::radians(rotation.y), glm::vec3(0.f, 1.f, 0.f)
        // );
        // model = glm::rotate(
        //     model, glm::radians(rotation.z), glm::vec3(0.f, 0.f, 1.f)
        // );
        // model = glm::scale(model, scale);
        return model;
    }
};
