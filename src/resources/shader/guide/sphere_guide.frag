#version 460

#define PI 3.14159265

layout(location = 0) in vec3 position;
layout(location = 1) in flat float fragActive;
layout(location = 2) in flat float fragLastUpdate;

layout(location = 0) out vec4 color;

layout (set = 0, binding = 0) uniform Matrices {
    mat4 view;
    mat4 viewProjection;
    mat4 inverseProjection;
    vec4 planes[6];
    float near;
    float far;
};

layout (set = 0, binding = 1) uniform Time {
    float currentTime;
};

layout (set = 0, binding = 2) uniform Scene {
    vec3 sceneCenter;
    vec3 sceneRadius;
    uint sectionsAmount;
    uint jointsAmount;
};

float gridAlpha (float entry, float widthScale) {
    return 1.0f - step(widthScale, abs(entry));
}

float timeAlpha () {
    const float DELAY = 0.2f;
    if (fragActive > 0.5f) {
        return smoothstep(0, 1, (currentTime - fragLastUpdate) / DELAY);
    } else {
        return 1.0f - smoothstep(0, 1, (currentTime - fragLastUpdate) / DELAY);
    }
}

void main() {
    vec4 x = vec4(0.8f, 0.5f, 0.5f, gridAlpha(position.x, 0.005f));
    vec4 y = vec4(0.5f, 0.8f, 0.5f, gridAlpha(position.y, 0.005f));
    vec4 z = vec4(0.5f, 0.5f, 0.8f, gridAlpha(position.z, 0.005f));

    vec4 result = mix(x, y, 1.0f - x.a);
    result = mix(result, z, 1.0f - result.a);
    result.a *= 0.5f * timeAlpha();
    color = result;
}