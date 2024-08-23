#version 460 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_color;
out vec4 v_color; // pumped to frag shader

void main()
{
    gl_Position = vec4(a_pos.x, a_pos.y, a_pos.z, 1.0);
    v_color = vec4(a_color.x, a_color.y, a_color.z, 1.0); // outputs the color input
}
