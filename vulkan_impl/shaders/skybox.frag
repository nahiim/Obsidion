

#version 450 core


layout(location = 0) in vec3 frag_texcoord;

layout(location = 0) out vec4 out_color;



layout(binding = 1) uniform samplerCube cubemap;



void main()
{    
    out_color = texture(cubemap, frag_texcoord);
}
