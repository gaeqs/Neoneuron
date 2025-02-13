#version 450

layout(location = 0) in vec2 vertex;

layout(location = 0) out vec2 texCoords;
layout(location = 1) out vec2 fragPosition;

void main() {
    gl_Position = vec4(vertex, 1.0f, 1.0f);
    texCoords = (vertex + 1.0f) / 2.0f;
    fragPosition = vertex;
}