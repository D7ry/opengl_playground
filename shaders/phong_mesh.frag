#version 330 core
in vec2 v_tex_coords;
in vec4 v_world_pos ;
in vec3 v_normal;


uniform sampler2D t_diffuse;
uniform sampler2D t_specular;
uniform sampler2D t_ambient;

// static point light defined as uniform
uniform vec3  u_light_position;
uniform vec3  u_light_color;
uniform float u_light_intensity;
uniform vec3 u_cam_pos;


uniform mat4 u_view; // specular camera pos calculation

// constant ambient light
const vec3  g_light_ambient_color = vec3(1.0);
const float g_light_ambient_intensity = 0.1;

const float g_shininess = 32.0; 

out vec4 o_color;
void main()
{
   vec3 material_diffuse = texture(t_diffuse, v_tex_coords).rgb;
   vec3 material_specular = texture(t_specular, v_tex_coords).rgb;

   vec3 ambient = vec3(0.0);  // ambient component
   vec3 diffuse = vec3(0.0);  // diffuse component
   vec3 specular = vec3(0.0); // specular component

   // light magnitude at light origin
   vec3 light = u_light_color * u_light_intensity;
   vec3 frag_to_light = u_light_position - v_world_pos.xyz;
   float d2l = length(frag_to_light);


   float attenuation = 1.0 / (1.0 + 0.09 * d2l + 0.032 * (d2l * d2l));

   { // ambient
       ambient = g_light_ambient_color * g_light_ambient_intensity * material_diffuse;
   }
   { // diffuse
       // CSE will do its job
       float costheta = dot(normalize(frag_to_light), normalize(v_normal));

       // between 90 and -90 deg, light is somewhat facing the surface
       if (costheta > 0) {
           // diffuse component
           diffuse = light * costheta * attenuation * material_diffuse;
       }
   }
   { // specular
       vec3 cam_pos = u_cam_pos;
       vec3 light_to_frag = normalize(-frag_to_light); // light dir
       // normalize will get propagated anyways by nv compiler
       vec3 light_reflect_dir = reflect(normalize(light_to_frag), normalize(v_normal.rgb));
       // cos between reflect dir and view dir is the magnitude
       vec3 frag_to_cam_dir = normalize(cam_pos - v_world_pos.xyz);
       float magnitude = dot(normalize(light_reflect_dir), normalize(frag_to_cam_dir));
       if (magnitude > 0) {
           magnitude = pow(magnitude, g_shininess);
           specular = magnitude * light * material_specular * attenuation;
       }
   }


   // output
   vec3 final_color = ambient + diffuse + specular;
   o_color = vec4(final_color, 1.0);
} 
