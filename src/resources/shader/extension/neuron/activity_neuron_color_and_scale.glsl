const int ACTIVITY_NEURON_COLOR_AND_SCALE_GRADIENT_SIZE = 256;

layout(set = EXTENSION_SET, binding = 0) uniform ActivityNeuronColorAndScale {
    vec4 gradient[ACTIVITY_NEURON_COLOR_AND_SCALE_GRADIENT_SIZE];
    vec4 scales[ACTIVITY_NEURON_COLOR_AND_SCALE_GRADIENT_SIZE / 4];
    float decay;
} activityNeuronColorAndScale;

layout(set = EXTENSION_SET, binding = 1) uniform ActivityNeuronColorAndScaleVolatile {
    float simulationTime;
} activityNeuronColorAndScaleVolatile;


layout(std430, set = EXTENSION_SET, binding = 2) buffer ActivityNeuronColorAndScaleActivity {
    float activity[];
} activityNeuronColorAndScaleActivity;

void fetchColorAndScale(uint extensionIndex, bool selected, out vec4 color, out float scale) {
    const int GRADIENT_SIZE_1 = ACTIVITY_NEURON_COLOR_AND_SCALE_GRADIENT_SIZE - 1;

    float value = activityNeuronColorAndScaleActivity.activity[extensionIndex];
    float time = (activityNeuronColorAndScaleVolatile.simulationTime - value) / activityNeuronColorAndScale.decay;
    if (time < 0) time = 1.0f;

    int index = clamp(int(floor(time * GRADIENT_SIZE_1)), 0, GRADIENT_SIZE_1);
    color = activityNeuronColorAndScale.gradient[index];
    scale = activityNeuronColorAndScale.scales[index / 4][index & 0x3];
}