#include "constants.h"
#include "camera.h"
#include <glm/gtc/matrix_transform.hpp> // lookat
#include "spdlog/spdlog.h"

// TODO: implement roll

// up vector of world space, pointing straight up

glm::mat4 Camera::get_view_matrix() { return this->view_matrix; }

void Camera::update_view_matrix() {
    glm::vec3 up = WORLD_UP_VECTOR;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw));
    direction.z = sin(glm::radians(yaw));
    direction.y = sin(glm::radians(pitch));  
    direction = glm::normalize(direction);

    // lookat handles the adjusting of the actual up vector internally
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

void Camera::mod_position(float x, float y, float z, bool follow_direction) {
    if (!follow_direction) {
        this->position.x += x;
        this->position.y += y;
        this->position.z += z;
    } else {
        glm::vec3 xyz = {x, y, z};

    }
    update_view_matrix();
}
