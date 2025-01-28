#version 460

layout(location = 0) in vec3 fragNormal;
layout(location = 1) flat in float fragNeuronId;
layout(location = 2) flat in float fragSectionId;
layout(location = 3) flat in float fragSelected;

layout(location = 0) out vec4 color;
layout(location = 1) out vec3 ids;

layout (set = 0, binding = 0) uniform Matrices {
    mat4 view;
    mat4 viewProjection;
    mat4 inverseProjection;
    vec4 planes[6];
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
    float somaWeightPower;
    float somaSphereWeight;
    float somaConnectionMaxWeight;
    vec4 defaultColor;
    vec4 selectedColor;
    uint lod;
};

void main() {
    vec4 viewNormal = view * vec4(normalize(fragNormal), 0.0f);
    float intensity = viewNormal.z * 0.8f + 0.2f;

    vec4 pre = fragSelected > 0.5f ? selectedColor : defaultColor;
    color = vec4(pre.xyz * intensity, pre.w);
    ids = vec3(1, fragNeuronId, fragSectionId);
}