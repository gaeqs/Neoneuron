#version 460

#define PI 3.14159265


layout(location = 0) in vec3 vertex;
layout(location = 1) in float isActive;
layout(location = 2) in float lastUpdate;
layout(location = 3) in vec3 center;

layout(location = 0) out vec3 position;
layout(location = 1) out flat float fragActive;
layout(location = 2) out flat float fragLastUpdate;

layout (set = 0, binding = 0) uniform Matrices {
    mat4 view;
    mat4 viewProjection;
    mat4 inverseProjection;
    vec4 planes[6];
    float near;
    float far;
};

layout (set = 0, binding = 2) uniform Scene {
    vec3 sceneCenter;
    vec3 sceneRadius;
};

void main() {
    gl_Position = viewProjection * vec4(center + vertex, 1.0f);
    gl_PointSize = 5.0f;
    position = vertex;
    fragActive = isActive;
    fragLastUpdate = lastUpdate;
}