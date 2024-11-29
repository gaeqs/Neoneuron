#version 460

layout(location = 0) in vec3 fragNormal;
layout(location = 1) flat in float fragType;
layout(location = 2) flat in float fragNeuronId;
layout(location = 3) flat in float fragSectionId;
layout(location = 4) flat in float fragSelected;

layout(location = 0) out vec4 color;
layout(location = 1) out vec3 ids;

layout (set = 0, binding = 0) uniform Matrices {
    mat4 view;
    mat4 viewProjection;
    mat4 inverseProjection;
    float near;
    float far;
};

void main() {
    vec4 viewNormal = view * vec4(fragNormal, 0.0f);
    float intensity = viewNormal.z * 0.8f + 0.2f;
    color = vec4(intensity, 0, 0, 1);

    ids = vec3(1, fragNeuronId, fragSectionId);
}