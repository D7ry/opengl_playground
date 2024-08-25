

#include "lab.h"
#include "camera.h"
#include "delta_time.h"
#include "input.h"

namespace Lab
{
namespace HelloTriangle
{
unsigned int vao;
SimpleShaderProgram* shaders = nullptr;
DeltaTimer delta_time;

Camera camera = Camera(
    {0.f, 0.f, 3.f}, // position
    0,
    0,
    0
);

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    static bool first_mouse_input = true;
    static double prev_x = 0;
    static double prev_y = 0;
    if (first_mouse_input) {
        first_mouse_input = false;
        prev_x = xpos;
        prev_y = ypos;
        return;
    }
    float offset_x = xpos - prev_x;
    float offset_y = ypos - prev_y;
    prev_x = xpos;
    prev_y = ypos;

    float sensitivity = 0.1f;
    offset_x *= sensitivity;
    offset_y *= sensitivity;

    camera.mod_yaw(offset_x);
    camera.mod_pitch(-offset_y);
}
} // namespace HelloTriangle
} // namespace Lab

void Lab::HelloTriangle::init(GLFWwindow* window) {
    INFO("Init Hello Triangle");
    glfwSetCursorPosCallback(window, mouse_callback);
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
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,    // top 
        
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,   // bottom left
         0.0f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f    // top 
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

        // bind movement inputs
        {
            static const int CAMERA_SPEED = 3;
            auto input = InputManager::get_singleton();
            input->register_key_callback(GLFW_KEY_ESCAPE, InputManager::KeyCallbackCondition::PRESS,
                [window]() {
                    glfwSetWindowShouldClose(window, true);
                }
            );
            input->register_key_callback(
                GLFW_KEY_W,
                InputManager::KeyCallbackCondition::HOLD,
                []() {
                    camera.mod_position(delta_time.get() * CAMERA_SPEED, 0, 0);
                }
            );
            input->register_key_callback(
                GLFW_KEY_S,
                InputManager::KeyCallbackCondition::HOLD,
                []() {
                    camera.mod_position(-delta_time.get() * CAMERA_SPEED, 0, 0);
                }
            );
            input->register_key_callback(
                GLFW_KEY_A,
                InputManager::KeyCallbackCondition::HOLD,
                []() {
                    camera.mod_position(0, 0, delta_time.get() * CAMERA_SPEED);
                }
            );
            input->register_key_callback(
                GLFW_KEY_D,
                InputManager::KeyCallbackCondition::HOLD,
                []() {
                    camera.mod_position(0, 0, -delta_time.get() * CAMERA_SPEED);
                }
            );
        }
    }

    { // imgui setup

    }
}

void Lab::HelloTriangle::tick() {
    delta_time.tick();
    InputManager::get_singleton()->tick(delta_time.get());

    shaders->use();

    { // transform the triangle
        // model matrix
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(
            model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f)
        );
        // view matrix
        glm::mat4 view = camera.get_view_matrix();
        // proj matrix
        glm::mat4 proj = glm::perspective(
            glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f
        );

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
        6             // count
    );
}
