

#version 330 core


in vec4 color;
in vec3 position;
in vec3 normal;

out vec4 FragColor;

uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform float ambience;
uniform vec3 camPos;
uniform vec3 specularStrength;
uniform float shininess;


void main()
{
    vec3 Normal = normalize(normal);
    
    vec3 lightDirection = normalize(lightPosition - position);

    vec3 ambient = ambience * color.xyz;

    float diff = max(dot(Normal, lightDirection), 0.0f);
    vec3 diffuse = diff * color.xyz;

    vec3 viewDirection = normalize(camPos - position);
    vec3 reflectionDirection = reflect(-lightDirection, Normal);

    float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), shininess);
    vec3 specular = specularStrength * specAmount * lightColor;

    FragColor = vec4(diffuse + ambient + specular, 1.0);
}