

#version 330 core  


layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 tex_coords;
layout (location = 2) in vec3 aNormal;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

out vec3 position;
out vec3 normal;

void main()
{
    position = aPos;
    normal = mat3(transpose(inverse(model))) * aNormal;

    vec4 pos = projection * view * model * vec4(position, 1.0);
    TexCoords = vec2(tex_coords.x, tex_coords.y);

    gl_Position = pos.xyzw;
}