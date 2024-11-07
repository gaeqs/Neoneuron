#version 460

#define PI 3.14159265


layout(location = 0) in vec3 vertex;
layout(location = 1) in float isActive;
layout(location = 2) in float lastUpdate;

layout(location = 0) out vec3 position;
layout(location = 1) out flat float fragActive;
layout(location = 2) out flat float fragLastUpdate;

layout (set = 0, binding = 0) uniform Matrices {
    mat4 view;
    mat4 viewProjection;
    mat4 inverseProjection;
    float near;
    float far;
};

layout (set = 0, binding = 2) uniform Scene {
    vec4 centerAndCameraRadius;
    vec3 radius;
    vec3 cameraCenter;
};

void main() {
    float radius = centerAndCameraRadius.w + 2.0f;
    gl_Position = viewProjection * vec4(cameraCenter + vertex * radius, 1.0f);

    float pitch = atan(vertex.y, vertex.x);
    float yaw = atan(vertex.z, vertex.x);

    position = vec3(pitch, yaw, -PI);
    fragActive = isActive;
    fragLastUpdate = lastUpdate;
}