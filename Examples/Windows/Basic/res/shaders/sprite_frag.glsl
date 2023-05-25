
#version 330 core

in vec2 f_texcoord;

out vec4 out_color;

uniform sampler2D sprite_texture;

void main()
{
    out_color = texture(sprite_texture, f_texcoord);
}