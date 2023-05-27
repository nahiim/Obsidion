

#version 330 core


layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec4 aColor;

out vec4 color;
out vec3 position;
out vec3 normal;

void main()
{
	position = aPos;
	color    = aColor;
	normal = mat3(transpose(inverse(model))) * aNormal;

	gl_Position = projection * view * model * vec4(position, 1.0);
}