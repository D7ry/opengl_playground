#pragma once

#include "glm/glm.hpp"

class Camera
{
  public:
    Camera() = delete;
    Camera(glm::vec3 position, float yaw, float pitch, float roll)
        : position(position),
          yaw(yaw),
          pitch(pitch),
          roll(roll){
              update_view_matrix();
          };
    glm::mat4 get_view_matrix();

    void mod_yaw(float yaw);

    void mod_pitch(float pitch);

  private:
    glm::vec3 position;

    float yaw;
    float pitch;
    float roll;
    glm::mat4 view_matrix;

    void update_view_matrix();
};
