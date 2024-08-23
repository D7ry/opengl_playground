#pragma once

#include "stdio.h"

// file io
#include <fstream>
#include <iostream>
#include <sstream>

#include "shader.h"

namespace Lab
{
namespace HelloTriangle
{
unsigned int vao;
unsigned int shader_program;

// init hellotriangle, set up buffers and send data to gpu
void init() {
    printf("Init Hello Triangle\n");
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
        printf("Vao created: %i\n", vao);
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
        shader_program = glCreateProgram();
        {
            Shader vertex_shader
                = Shader("../shaders/hello_triangle.vert", GL_VERTEX_SHADER);
            Shader fragment_shader
                = Shader("../shaders/hello_triangle.frag", GL_FRAGMENT_SHADER);

            bool success = vertex_shader.compile() && fragment_shader.compile();
            if (!success) {
                printf("Error: shader compilation failed\n");
            }

            // attach shaders
            glAttachShader(shader_program, vertex_shader.shader_id);
            glAttachShader(shader_program, fragment_shader.shader_id);
            glLinkProgram(shader_program);
        }
    }
}

void tick() {
    // enable shader program
    glUseProgram(shader_program);
    // bind vao
    glBindVertexArray(vao);
    // draw triangles. currently we have 3 triangles
    glDrawArrays(
        GL_TRIANGLES, // mode
        0,            // first
        3             // count
    );
}
} // namespace HelloTriangle

} // namespace Lab
