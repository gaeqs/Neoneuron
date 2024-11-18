//
// Created by gaeqs on 15/11/24.
//

#ifndef NEURONSELECTOR_H
#define NEURONSELECTOR_H

#include <vector>
#include <neon/render/shader/ShaderUniformBuffer.h>
#include <neoneuron/Types.h>
#include <neoneuron/render/AbstractSelector.h>

namespace neoneuron {
    class NeuronScene;

    struct SelectionEntry {
        UID neuron;
        UID segment;
    };

    struct GPUNeuronSelectionData {
        uint8_t selected;
    };

    class NeuronSelector : public AbstractSelector {
        NeuronScene* _scene;
        neon::ShaderUniformBuffer* _uniformBuffer;
        size_t _binding;

        std::unordered_map<SelectionEntry, size_t> _selection;

    public:
        NeuronSelector(NeuronScene* scene,
                       neon::ShaderUniformBuffer* uniformBuffer,
                       size_t binding);

        ~NeuronSelector() override;

        void setSelectionData(const void* data) override;
    };
}

namespace std {
    struct hash<neoneuron::SelectionEntry> {
        size_t operator()(const neoneuron::SelectionEntry& entry) const {
            std::hash<uint8_t> hasher;
            size_t hash1 = hasher(entry.neuron);
            size_t hash2 = hasher(entry.segment);
            return hash1 ^ hash2 + 0x9e3779b9 + (hash1 << 6) + (hash1 >> 2);
        }
    };
}

#endif //NEURONSELECTOR_H
