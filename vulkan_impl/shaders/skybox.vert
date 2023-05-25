
#version 450 core


layout(location = 0) in vec3 in_position;
layout(location = 2) in vec3 in_tex_coord;

layout(location = 0) out vec3 frag_texcoord;



layout(binding = 0) uniform ubo
{
    mat4 viewMatrix;
    mat4 projectionMatrix;
} scene_data;



void main()
{
    frag_texcoord = in_tex_coord;
    vec4 position = scene_data.projectionMatrix * scene_data.viewMatrix * vec4(in_position, 1.0);
    gl_Position = position.xyww;
}  

