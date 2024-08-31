#include "phong_shading.h"
#include "shader.h"
#include "camera.h"
#include "mesh.h"

void PhongShadingApp::init(InitData& init_data) {
    this->shader = std::make_unique<SimpleShaderProgram>(
        "../shaders/phong_mesh.vert", "../shaders/phong_mesh.frag"
    );
    this->shader->build();
    this->model = std::make_unique<PhongModel>("../assets/backpack.obj", init_data.texture_manager);
}

void PhongShadingApp::tick(TickData& tick_data) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(0.f), glm::vec3(1.0f, 0.0f, 0.0f));
    // view matrix
    glm::mat4 view = tick_data.camera->get_view_matrix();
    // proj matrix
    glm::mat4 proj = glm::perspective(
        glm::radians(FOV),
        (float)VIEWPORT_WIDTH / (float)VIEWPORT_HEIGHT,
        0.1f,
        100.0f
    );
    this->model->draw(model, view, proj, tick_data.camera->get_position(), *this->shader);
}

