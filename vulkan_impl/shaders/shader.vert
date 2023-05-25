
#version 450


layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_color;

layout(location = 0) out vec3 frag_color;




layout(push_constant) uniform constants
{
	mat4 transformMatrix;
} mesh_data;


layout(binding = 0) uniform ubo
{
	mat4 viewMatrix;
	mat4 projectionMatrix;
} scene_data;



void main()
{
	gl_Position = scene_data.projectionMatrix * scene_data.viewMatrix * mesh_data.transformMatrix * vec4(in_position, 1.0);
	frag_color = in_color;
}