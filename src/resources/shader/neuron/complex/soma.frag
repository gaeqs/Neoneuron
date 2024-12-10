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
    vec4 defaultColor;
    vec4 selectedColor;
};

void main() {
    vec4 viewNormal = view * vec4(normalize(fragNormal), 0.0f);
    float intensity = viewNormal.z * 0.8f + 0.2f;

    uint type = floatBitsToUint(fragType);
    vec3 color3 = defaultColor.xyz;
    if(type == 0) {
        color3 = vec3(0, 1, 0);
    } else if(type == 1) {
        color3 = vec3(0, 0, 1);
    } else if (type == 2) {
        color3 = vec3(1, 1, 0);
    } else if(type == 3) {
        color3 = vec3(1, 1, 1);
    } else if(type == 4) {
        color3 = vec3(0.5f, 0.5f, 0.5f);
    } else if(type == 5) {
        color3 = vec3(1, 0.5f, 0.5f);
    } else if(type == 6) {
        color3 = vec3(0.5f, 1, 0.5f);
    } else if(type == 7) {
        color3 = vec3(0.5f, 0.5f, 1);
    }

    color = vec4(color3 * intensity, 1.0f);
    ids = vec3(1, fragNeuronId, fragSectionId);
}