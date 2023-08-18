#version 450 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D Texture;

void main() {
    vec3 col = texture(Texture, TexCoords).xyz;
    FragColor = vec4(col, TexCoords);
}