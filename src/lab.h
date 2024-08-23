#pragma once

#include "stdio.h"

// file io
#include <fstream>
#include <iostream>
#include <sstream>

#include "shader.h"
namespace Lab
{
void hello_triangle() {

    // clang-format off

    // 3 vertices of a triangle
    float vertices[]
        = {-0.5f, -0.5f, 0.0f, 
            0.5f, -0.5f, 0.0f, 
            0.0f, 0.5f, 0.0f};
    // clang-format on

    // generate one buffer
    unsigned int vbo;
    glGenBuffers(1, &vbo);

    // bind the buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // after binding, subsequent calls that operates on `GL_ARRAY_BUFFER`,
    // operates on `vbo`

    // now we copy the vertices over
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(vertices), // num of bytes to copy over
        &vbo,             // ptr to host memory
        GL_STATIC_DRAW    // update once, draw many times
    );

    Shader vertex_shader = Shader("../shaders/hello_triangle.vert", GL_VERTEX_SHADER);
    Shader fragment_shader = Shader("../shaders/hello_triangle.frag", GL_FRAGMENT_SHADER);
    bool success = vertex_shader.compile() && fragment_shader.compile();
    if (!success) { 
        printf("Error: shader compilation failed\n");
    }

}
} // namespace Lab
