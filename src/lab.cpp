#include "lab.h"
#include "spdlog/spdlog.h"

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp> // rotate and translate

namespace Lab
{
namespace HelloTriangle
{
unsigned int vao;
SimpleShaderProgram* shaders = nullptr;
} // namespace HelloTriangle
} // namespace Lab

void Lab::HelloTriangle::init() {
    INFO("Init Hello Triangle");
    // clang-format off

    // 3 vertices of a triangle
    // float vertices[]
    //     = {-0.5f, -0.5f, 0.0f, 
    //         0.5f, -0.5f, 0.0f, 
    //         0.0f, 0.5f, 0.0f};
    float vertices[] = {
        // positions         // colors
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
    };
    // clang-format on

    { // vao
        // generate one vertex array object
        glGenVertexArrays(1, &vao);
        SPDLOG_INFO("Vao created: {}", vao);
        // bind vertex array
        glBindVertexArray(vao);
        // after binding, subsequent calls operate on `vao`
        { // vertex attr at location 0 and 1
            // buffer
            {
                unsigned int vbo;
                glGenBuffers(1, &vbo);
                glBindBuffer(GL_ARRAY_BUFFER, vbo);
                // copy the vertices data to buffer
                glBufferData(
                    GL_ARRAY_BUFFER,
                    sizeof(vertices), // num of bytes to copy over
                    vertices,         // ptr to host memory
                    GL_STATIC_DRAW    // update once, draw many times
                );
            }
            // set up vertex attribute arrs
            { // location = 0
                glVertexAttribPointer(
                    0, // location
                    3, // size of one vertex : how many elems in one vertex
                       // attribute
                    GL_FLOAT, // data type of a single elem
                    GL_FALSE, // only needed for int data type to normalize to
                              // [0, 1]
                    6 * sizeof(float), // stride between vertices, 6 since we
                                       // have 3 floats to store rgb info
                    (void*)0           // offset
                );
                glVertexAttribPointer(
                    1, // location
                    3, // size of one vertex : how many elems in one vertex
                       // attribute
                    GL_FLOAT, // data type of a single elem
                    GL_FALSE, // only needed for int data type to normalize to
                              // [0, 1]
                    6 * sizeof(float),         // stride between vertices
                    (void*)(3 * sizeof(float)) // offset
                );
                glEnableVertexAttribArray(0); // enable pointer at location 0
                glEnableVertexAttribArray(1); // enable pointer at location 1
            }
        }

        // shader setup
        // create & compile shader program
        shaders = new SimpleShaderProgram(
            "../shaders/hello_triangle.vert", "../shaders/hello_triangle.frag"
        );
        bool success = shaders->build();
        if (!success) {
            ERROR("Shader program building failed");
        }
    }
}

void Lab::HelloTriangle::tick() {
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    shaders->use();

    { // transform the triangle
        // model matrix
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f)); 
        // view matrix
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f)); 
        // proj matrix
        glm::mat4 proj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

        // set transform matrices
        shaders->set_uniform_mat4("u_model", model);
        shaders->set_uniform_mat4("u_view", view);
        shaders->set_uniform_mat4("u_proj", proj);

    }

    // bind vao
    glBindVertexArray(vao);
    // draw triangles. currently we have 3 triangles
    glDrawArrays(
        GL_TRIANGLES, // mode
        0,            // first
        3             // count
    );
}
