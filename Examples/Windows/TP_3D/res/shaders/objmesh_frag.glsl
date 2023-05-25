

#version 330 core


in vec2 TexCoords;
in vec3 position;
in vec3 normal;

uniform sampler2D sampler;

uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform float ambience;

out vec4 FragColor;

void main()
{    
    vec3 Normal = normalize(normal);
    
    vec3 lightDirection = normalize(lightPosition - position);

    float diffuse = max(dot(Normal, lightDirection), 0.0f);

    FragColor = texture(sampler, TexCoords) * vec4(lightColor, 1.0f) * (diffuse + ambience);
}