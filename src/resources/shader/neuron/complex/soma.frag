#version 460

layout(location = 0) in vec3 fragNormal;
layout(location = 1) in vec3 fragColor;
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

layout (set = 0, binding = 1) uniform GlobalData {
    float currentTime;
    float radiusStrength;
    float startClip;
    float endClip;
    float splitHeight;
    float splitArcStrength;
    uint rotationIndexOffset;
    uint childrenRotationIndexOffset;
    uint minChildrenForJoint;
    uint verticesPerCircle;
    uint somaLatitudes;
    uint somaLongitudes;
    float somaConnectionPushFactor;
    vec4 defaultColor;
    vec4 selectedColor;
};

void main() {
    vec4 viewNormal = view * vec4(normalize(fragNormal), 0.0f);
    float intensity = viewNormal.z * 0.8f + 0.2f;

    color = vec4(fragColor * intensity, 1.0f);
    ids = vec3(1, fragNeuronId, fragSectionId);
}