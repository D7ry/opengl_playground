#include "camera.h"
#include "constants.h"
#include "spdlog/spdlog.h"
#include <glm/gtc/matrix_transform.hpp> // lookat

// TODO: implement roll

glm::mat4 Camera::get_view_matrix() { return this->view_matrix; }

void Camera::update_view_matrix() {
    glm::vec3 up = WORLD_UP_VECTOR;
    glm::vec3 direction;
    float pitch_rad = glm::radians(pitch);
    float yaw_rad = glm::radians(yaw);
    
    direction.x = cos(yaw_rad);
    direction.z = sin(yaw_rad);

    // adjust y component to x and z
    direction.y = tan(pitch_rad) * glm::length(glm::vec2{direction.x, direction.z});
    
    direction = glm::normalize(direction);
    
    this->view_matrix = glm::lookAt(this->position, this->position + direction, up);
    return;
}

void Camera::mod_yaw(float yaw) {
    this->yaw += yaw;
    // wrap yaw between -180 and 180
    if (this->yaw < -180.0f) {
        this->yaw += 360.0f;
    } else if (this->yaw > 180.0f) {
        this->yaw -= 360.0f;
    }
    update_view_matrix();
}

void Camera::mod_pitch(float pitch) {
    this->pitch += pitch;
    this->pitch = glm::clamp(this->pitch, -89.f, 89.f);
    update_view_matrix();
}

void Camera::mod_position(
    float x,
    float y,
    float z,
    bool follow_yaw,
    bool follow_pitch
) {
    // swap x and z to align the camera axis with world space
    std::swap(x, z);
    x = -x; // adjust camera forward sign
    glm::vec3 offset(x, y, z);
    if (follow_yaw) {
        float yaw_rad = glm::radians(this->yaw);
        glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), -yaw_rad, glm::vec3(0.0f, 1.0f, 0.0f));
        offset = glm::vec3(rotation * glm::vec4(offset, 0.0f));
    }

    if (follow_pitch) {
        float pitch_rad = glm::radians(this->pitch);
        glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), pitch_rad, glm::vec3(1.0f, 0.0f, 0.0f));
        offset = glm::vec3(rotation * glm::vec4(offset, 0.0f));
    }

    this->position += offset;
    update_view_matrix();
}
