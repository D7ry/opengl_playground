#include "camera.h"
#include "coordinate_system.h"

void CoordinateSystemApp::init(InitData& init_data) {
    // clang-format off
        float coordinate_vertices[] = {
        // Positions        // Colors
        -10.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // X-axis (red)
         10.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
         0.0f, -10.0f, 0.0f, 0.0f, 1.0f, 0.0f, // Y-axis (green)
         0.0f,  10.0f, 0.0f, 0.0f, 1.0f, 0.0f,
         0.0f, 0.0f, -10.0f, 0.0f, 0.0f, 1.0f, // Z-axis (blue)
         0.0f, 0.0f,  10.0f, 0.0f, 0.0f, 1.0f
        };
    // clang-format on
    //
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    { // set up vao
        glGenBuffers(1, &this->vbo);
        glBindBuffer(GL_ARRAY_BUFFER, this->vbo);

        // copy to device
        glBufferData(
            GL_ARRAY_BUFFER,
            sizeof(coordinate_vertices),
            coordinate_vertices,
            GL_STATIC_DRAW
        );

        // location = 0
        glVertexAttribPointer(
            0,        // location
            3,        // size of one vertex : how many elems in one vertex
                      // attribute
            GL_FLOAT, // data type of a single elem
            GL_FALSE, // only needed for int data type to normalize to
                      // [0, 1]
            6 * sizeof(float), // stride between vertices, 6 since we
                               // have 3 floats to store rgb info
            (void*)0           // offset
        );

        // location = 1
        glVertexAttribPointer(
            1,        // location
            3,        // size of one vertex : how many elems in one vertex
                      // attribute
            GL_FLOAT, // data type of a single elem
            GL_FALSE, // only needed for int data type to normalize to
                      // [0, 1]
            6 * sizeof(float),         // stride between vertices, 6 since we
                                       // have 3 floats to store rgb info
            (void*)(3 * sizeof(float)) // offset
        );
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
    }
    glBindVertexArray(0);

    this->shader = std::make_unique<SimpleShaderProgram>(
        "../shaders/apps/coordinate_system.vert",
        "../shaders/apps/coordinate_system.frag"
    );
    bool success = this->shader->build();
    if (!success) {
        CRIT("Shader program building failed");
    }
};

void CoordinateSystemApp::tick(TickData& tick_data) {
    this->shader->use();
    glm::mat4 model = glm::mat4(1.f); // no need to model transform
    shader->set_uniform_mat4(U_MODEL, model);
    shader->set_uniform_mat4(U_VIEW, tick_data.camera->get_view_matrix());
    shader->set_uniform_mat4(U_PROJ, tick_data.proj);

    glBindVertexArray(this->vao);
    glDrawArrays(GL_LINES, 0, 6); // draw xyz, each line takes 2 indices
    glBindVertexArray(0);
}

CoordinateSystemApp::~CoordinateSystemApp() {
    if (vao)
        glDeleteVertexArrays(1, &vao);
    if (vbo)
        glDeleteBuffers(1, &vbo);
}
