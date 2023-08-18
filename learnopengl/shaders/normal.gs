#version 450 core

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in VS_OUT {
    vec3 normal;
} gs_in[];

const float MAGNITUDE = 0.4;

uniform mat4 projection;

void CalcNormal(int id) {
    gl_Position = projection * gl_in[id].gl_Position;
    EmitVertex();

    gl_Position = projection * vec4(gl_in[id].gl_Position + vec4(gs_in[id].normal, 0.0) * MAGNITUDE);
    EmitVertex();
    
    EndPrimitive();
}

void main() {
    CalcNormal(0);
    CalcNormal(1);
    CalcNormal(2);
}