#version 450 core

out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} fs_in;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;

vec3 BlinnPhong(vec3 normal, vec3 fragPos, vec3 lightPos, vec3 lightColor) {
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 viewDir = normalize(fs_in.TangentViewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(halfwayDir, normal), 0.0), 64.0);
    vec3 specular = vec3(0.2) * spec;

    float max_distance = 1.5;
    float distance = length(lightPos - fragPos);
    float attenuation = 1.0 / distance * distance;

    diffuse *= attenuation;
    specular *= attenuation;

    return diffuse + specular;
}

void main()
{           
    vec3 color = texture(texture_diffuse1, fs_in.TexCoords).rgb;
    // ambient
    vec3 ambient = 0.1 * color;
    vec3 normal = normalize(texture(texture_normal1, fs_in.TexCoords).rgb * 2.0 - 1.0);
    
    vec3 lighting = ambient;
    lighting += BlinnPhong(normal, fs_in.TangentFragPos, fs_in.TangentLightPos, color);
    color *= lighting;

    color = pow(color, vec3(1.0 / 2.2));

    FragColor = vec4(color, 1.0);
}