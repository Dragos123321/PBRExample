#version 450 core
 
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out VS_OUT {
    vec3 normal;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat3 normalTransform;

void main() {
    gl_Position = view * model * vec4(aPos, 1.0);
    vs_out.normal = normalize(vec3(vec4(normalTransform * aNormal, 0.0)));
}