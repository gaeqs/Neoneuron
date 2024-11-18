//
// Created by gaeqs on 15/11/24.
//

#include "NeuronSelector.h"

#include "NeuronScene.h"

namespace neoneuron {
    NeuronSelector::NeuronSelector(NeuronScene* scene, neon::ShaderUniformBuffer* uniformBuffer, size_t binding)
        : _scene(scene),
          _uniformBuffer(uniformBuffer),
          _binding(binding) {}

    void NeuronSelector::setSelectionData(const void* data) {
        const UID* uids = static_cast<const UID*>(data);
        UID neuron = *uids;
        UID section = *(uids + 1);

        SelectionEntry entry{neuron, section};
        if (_selection.contains(entry)) return;

        auto neuron = _scene->getNeurons();
    }
}
