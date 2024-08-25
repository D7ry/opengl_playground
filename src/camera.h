#pragma once

#include "glm/glm.hpp"

class Camera
{
  public:
    Camera() = delete;

    // yaw pitch roll are in degree
    Camera(glm::vec3 position, float yaw, float pitch, float roll)
        : position(position), yaw(yaw), pitch(pitch), roll(roll) {
        update_view_matrix();
    };

    glm::mat4 get_view_matrix();

    void mod_yaw(float yaw);
    void mod_pitch(float pitch);

    // modify the camera position
    void mod_position(
        float x, // +x = right
        float y,
        float z, // -z = forward
        bool follow_yaw = true,
        bool follow_pitch = false // FIXME: fix pitch following for sideways movement
    );

    glm::vec3 get_position() { return position; }

    glm::vec3 get_yaw_pitch_roll() { return glm::vec3{yaw, pitch, roll}; }

  private:
    glm::vec3 position;

    float yaw;
    float pitch;
    float roll;
    glm::mat4 view_matrix;

    void update_view_matrix();
};
