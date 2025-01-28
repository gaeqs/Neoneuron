#version 460

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
    vec3 center;
    vec3 radius;
};

float gridAlpha (float scale, float widthScale) {
    vec3 lineAA = fwidth(position / scale) * 1.5f;
    vec3 lineUV = 1.0 - abs(fract(position / scale) * 2.0 - 1.0);
    float width = 0.05f / widthScale;
    vec3 grid = smoothstep(width + lineAA, width - lineAA, lineUV);
    return max(grid.x, grid.z);
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


    vec4 big = vec4(0.8f, 0.5f, 0.5f, gridAlpha(10, 2));
    vec4 small = vec4(0.5f, 0.5f, 0.5f, gridAlpha(1, 1));
    vec4 result = mix(big, small, 1.0f - big.a);
    result.a *= 0.2f * timeAlpha();
    color = result;
}