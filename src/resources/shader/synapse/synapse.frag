#version 460

layout(location = 0) flat in uint fragSynapseType;
layout(location = 1) in vec3 fragCenter;
layout(location = 2) in vec3 fragPosition;

layout(location = 0) out vec4 color;
layout(location = 1) out vec3 ids;

const float PARTICLE_RADIUS = 1.5f;

void main() {
    vec3 d = fragPosition - fragCenter;
    if (dot(d, d) > PARTICLE_RADIUS * PARTICLE_RADIUS) {
        discard;
    }

    if (fragSynapseType == 0) {
        // Presynaptic
        color = vec4(1.0f, 0.58f, 0.01f, 1.0f);
    } else {
        // Postsynaptic
        color = vec4(0, 0.22f, 1.0f, 1.0f);
    }
    ids = vec3(0, 0, 0);
}