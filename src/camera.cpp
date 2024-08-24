#include "camera.h"
#include <glm/gtc/matrix_transform.hpp> // lookat
#include "spdlog/spdlog.h"

glm::mat4 Camera::get_view_matrix() { return this->view_matrix; }

void Camera::update_view_matrix() {
    INFO("yaw: {} pitch: {}", this->yaw, this->pitch);

    // figure out front and up vector using yaw and pitch
    glm::vec3 up = {0.f, 1.f, 0.f};
    glm::vec3 direction;

    direction.x = cos(glm::radians(yaw));
    direction.z = sin(glm::radians(yaw));
    direction.y = sin(glm::radians(pitch));  
    direction = glm::normalize(direction);

    this->view_matrix = glm::lookAt(this->position, this->position + direction, up);
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
