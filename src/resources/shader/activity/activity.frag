#version 460

layout(location = 0) in vec3 fragCenter;
layout(location = 1) in vec3 fragPosition;
layout(location = 2) in vec4 fragColor;
layout(location = 3) in float fragSize;

layout(location = 0) out vec4 color;
layout(location = 1) out vec3 ids;


void main() {
    vec3 d = fragPosition - fragCenter;
    if (dot(d, d) > fragSize * fragSize) {
        discard;
    }

    color = fragColor;
    ids = vec3(0, 0, 0);
}