#version 460 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_color;
out vec4 v_color; // pumped to frag shader

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;


void main()
{
    gl_Position =  u_proj * u_view * u_model * vec4(a_pos, 1.0f);
    v_color = vec4(a_color.x, a_color.y, a_color.z, 1.0); // outputs the color input
}
