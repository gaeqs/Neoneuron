#version 460

layout(location = 0) in vec3 fragNormal;
layout(location = 1) flat in float fragType;
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

layout (set = 0, binding = 1) uniform GlobalData {
    float currentTime;
    float radiusStrength;
    float somaRadiusStrength;
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
    uint lod;
    uint frame;
    uint savingNeuron;
    vec4 defaultColor;
    vec4 selectedColor;
};

vec4 shading(vec3 color_, vec3 normal_, vec3 L_) {
    vec3 N = normalize(normal_);
    float dif = clamp(dot(N, L_), 0.0, 1.0);
    return vec4(color_ * dif, 1.0) + vec4(color_ * 0.3, 1.0);
}

void main() {
    vec4 viewNormal = view * vec4(normalize(fragNormal), 0.0f);
    float intensity = clamp(viewNormal.z, 0.0f, 1.0f) * 0.7f + 0.3f;

    uint type = floatBitsToUint(fragType);
    vec4 pre = fragSelected > 0.5f ? selectedColor : defaultColor;
    color = vec4(pre.xyz * intensity, pre.w);
    ids = vec3(uintBitsToFloat(floatBitsToUint(fragDatasetId) + 1), fragNeuronId, fragSectionId);
}