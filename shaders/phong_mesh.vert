#version 330 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_tex_coords;

// pumped to frag shader
out vec2 v_tex_coords;
out vec4 v_world_pos;
out vec3 v_normal;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;

void main()
{
    // position is simply object mat
    vec4 world_pos = u_model * vec4(a_pos, 1.0f);
    gl_Position = u_proj * u_view * world_pos;
    // outputs values for frag shader
    v_tex_coords = a_tex_coords;
    v_world_pos = world_pos; // perspective division automatically done by glslc
    v_normal = a_normal;
}
