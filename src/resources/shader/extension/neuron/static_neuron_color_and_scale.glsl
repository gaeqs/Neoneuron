layout(set = EXTENSION_SET, binding = 0) uniform StaticNeuronColorAndScale {
    vec4 color;
    vec4 selectedColor;
    float scale;
    float selectedScale;
} staticNeuronColorAndScale;


void fetchColorAndScale(uint extensionIndex, bool selected, out vec4 color, out float scale) {
    if(selected) {
        color = staticNeuronColorAndScale.selectedColor;
        scale = staticNeuronColorAndScale.selectedScale;
    } else {
        color = staticNeuronColorAndScale.color;
        scale = staticNeuronColorAndScale.scale;
    }
}