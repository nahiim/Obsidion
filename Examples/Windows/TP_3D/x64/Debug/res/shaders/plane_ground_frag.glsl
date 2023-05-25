

#version 330 core



uniform sampler2D diffuseMap;  // 0
uniform sampler2D normalMap;   // 1
uniform sampler2D shadowMap;    // 2

uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform float ambience;
uniform vec3 camPos;

in vec4 ccc;

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;

    vec3 Normal;
    vec4 FragPosLightSpace;
} fs_in; 


out vec4 FragColor;


float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightDir = normalize(lightPosition - fs_in.FragPos);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    // check whether current frag pos is in shadow
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
    return shadow;
}


void main()
{   
     // obtain normal from normal map in range [0,1]
    vec3 Normal = texture(normalMap, fs_in.TexCoords).rgb;

    // transform normal vector to range [-1,1]
    Normal = normalize(Normal * 2.0 - 1.0);  // this normal is in tangent space

    // get diffuse color
    vec3 color = texture(diffuseMap, fs_in.TexCoords).rgb;

    // ambient
    vec3 ambient = ambience * color;
    
    vec3 lightDirection = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
    float diff = max(dot(lightDirection, Normal), 0.0);
    vec3 diffuse = diff * color;

    vec3 viewDirection = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    vec3 reflectionDirection = reflect(-lightDirection, Normal);
    vec3 halfwayDir = normalize(viewDirection + viewDirection);
    float spec = pow(max(dot(Normal, halfwayDir), 0.0), 32.0);
    vec3 specular = vec3(0.5, 0.5, 0.5) * spec * lightColor;

    // calculate shadow
    float shadow = ShadowCalculation(fs_in.FragPosLightSpace); 

//    FragColor = vec4(ambient + diffuse + specular, 1.0);
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;    
    
    FragColor = vec4(lighting, 1.0);

//FragColor = ccc;

}