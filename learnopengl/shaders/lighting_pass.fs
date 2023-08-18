#version 450 core

#define NR_LIGHTS 32

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform vec3 viewPos;

struct Light {
    vec3 Position;
    vec3 Color;

    float Linear;
    float Quadratic;
    float Radius;
};

uniform Light lights[NR_LIGHTS];

void main() {
    vec3 fragPos = texture(gPosition, TexCoords).rgb;
    vec3 normal = texture(gNormal, TexCoords).rgb;
    vec3 diffuse = texture(gAlbedoSpec, TexCoords).rgb;
    float specular = texture(gAlbedoSpec, TexCoords).a;

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 lighting = 0.1 * diffuse;

    for (uint i = 0; i < NR_LIGHTS; ++i) {
        float distance = length(lights[i].Position - fragPos);

        if (distance < lights[i].Radius) {
            vec3 lightDir = normalize(lights[i].Position - fragPos);
            vec3 diff = max(dot(normal, lightDir), 0.0) * diffuse * lights[i].Color;

            vec3 halfway = normalize(lightDir + viewDir);
            vec3 spec = pow(max(dot(normal, halfway), 0.0), 32.0) * specular * lights[i].Color;

            float attenuation = 1.0 / (1.0 + lights[i].Linear * distance + lights[i].Quadratic * distance * distance);
            diff *= attenuation;
            spec *= attenuation;
            lighting += diff + spec; 
        }
    }

    FragColor = vec4(lighting, 1.0);
}