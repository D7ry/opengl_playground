#version 330 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_tex_coords;

// pumped to frag shader
out vec2 v_tex_coords;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;

void main()
{
    // position is simply object mat
    gl_Position =  u_proj * u_view * u_model * vec4(a_pos, 1.0f);
    // outputs normal
    v_tex_coords = a_tex_coords;
}
