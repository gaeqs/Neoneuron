#version 460

layout(location = 0) in vec3 fragNormal;
layout(location = 1) flat in vec4 fragColor;
layout(location = 2) flat in float fragDatasetId;
layout(location = 3) flat in float fragNeuronId;
layout(location = 4) flat in float fragSectionId;
layout(location = 5) flat in float fragSelected;

layout(location = 0) out vec4 color;
layout(location = 1) out vec3 ids;

layout (set = 1, binding = 0) uniform Matrices {
    mat4 view;
    mat4 viewProjection;
    mat4 inverseProjection;
    vec4 planes[6];
    float near;
    float far;
};

void main() {
    vec4 viewNormal = view * vec4(normalize(fragNormal), 0.0f);
    float intensity = clamp(viewNormal.z, 0.0f, 1.0f) * 0.7f + 0.3f;

    color = vec4(fragColor.xyz * intensity, fragColor.w);
    ids = vec3(uintBitsToFloat(floatBitsToUint(fragDatasetId) + 1), fragNeuronId, fragSectionId);
}