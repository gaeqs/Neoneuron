#version 460

layout(location = 0) in vec2 texCoords;
layout(location = 1) in vec2 fragPosition;

layout (set = 0, binding = 0) uniform Matrices {
    mat4 view;
    mat4 viewProjection;
    mat4 inverseProjection;
    vec4 planes[6];
    float near;
    float far;
};

layout(set = 1, binding = 0) uniform sampler2D colorTexture;
layout(set = 1, binding = 1) uniform sampler2DMS selectorMultisampled;
layout(set = 1, binding = 2) uniform samplerCube skybox;
layout(set = 1, binding = 3) uniform SkyboxData
{
    uint skyboxPresent;
};

layout(location = 0) out vec4 colorResult;
layout(location = 1) out vec4 selectorResult;

void main() {
    ivec2 texSize = textureSize(selectorMultisampled);
    ivec2 texelCoord = ivec2(texCoords * texSize);

    // Resolve color
    vec4 color = texelFetch(colorTexture, texelCoord, 0);
    vec3 background;

    if (skyboxPresent > 0) {
        vec4 projectedPosition = vec4(fragPosition, 1.0f, 1.0f);
        vec4 position4 = inverseProjection * projectedPosition;
        vec3 position = position4.xyz / position4.w;
        mat3 inverseView = transpose(mat3(view));
        background = texture(skybox, inverseView * position).rgb;
    } else {
        background = vec3(1.0f, 1.0f, 1.0f);
    }
    colorResult = vec4(mix(background, color.rgb, color.a), 1.0f);



    // Resolve selection
    selectorResult = texelFetch(selectorMultisampled, texelCoord, 0);
}