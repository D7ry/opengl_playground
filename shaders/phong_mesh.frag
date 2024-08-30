#version 330 core
in vec2 v_tex_coords;

out vec4 o_color;

uniform sampler2D t_diffuse;
uniform sampler2D t_specular;

void main()
{
   vec3 diffuse = texture(t_diffuse, v_tex_coords).rgb;

   o_color = vec4(diffuse, 1.0);
} 
