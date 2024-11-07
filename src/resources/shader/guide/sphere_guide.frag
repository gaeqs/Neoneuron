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
    float near;
    float far;
};

layout (set = 0, binding = 1) uniform Time {
    float currentTime;
};

layout (set = 0, binding = 2) uniform Scene {
    vec4 centerAndCameraRadius;
    vec3 radius;
    vec3 cameraCenter;
};

float gridAlpha (float scale, float widthScale) {
    vec3 grid = fract(position * scale);
    return step(widthScale, grid.x) * step(widthScale, grid.y);
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
    vec4 big = vec4(0.8f, 0.5f, 0.5f, gridAlpha(10, 0.2));
    vec4 small = vec4(0.5f, 0.5f, 0.5f, gridAlpha(1, 0.1));
    vec4 result = mix(big, small, 1.0f - big.a);
    result.a *= 0.2f; //* timeAlpha();
    color = result;//vec4((position + PI) / (2 * PI), 1.0f);
}