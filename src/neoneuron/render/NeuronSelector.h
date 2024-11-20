//
// Created by gaeqs on 15/11/24.
//

#ifndef NEURONSELECTOR_H
#define NEURONSELECTOR_H

#include <unordered_map>
#include <neon/render/model/InstanceData.h>

#include <neon/render/shader/ShaderUniformBuffer.h>
#include <neoneuron/Types.h>
#include <neoneuron/render/AbstractSelector.h>


namespace neoneuron {
    struct SelectionEntry {
        UID neuron;
        UID segment;

        bool operator==(const SelectionEntry& other) const;

        bool operator!=(const SelectionEntry& other) const;
    };
}

template<>
struct std::hash<neoneuron::SelectionEntry> {
    size_t operator()(const neoneuron::SelectionEntry& entry) const noexcept {
        std::hash<uint32_t> hasher;
        size_t hash1 = hasher(entry.neuron);
        size_t hash2 = hasher(entry.segment);
        return hash1 ^ hash2 + 0x9e3779b9 + (hash1 << 6) + (hash1 >> 2);
    }
};

namespace neoneuron {
    class NeuronScene;

    struct GPUNeuronSelectionData {
        uint32_t selected;
    };

    class NeuronSelector : public AbstractSelector {
        NeuronScene* _scene;
        neon::ShaderUniformBuffer* _uniformBuffer;
        size_t _binding;

        std::unordered_map<SelectionEntry, neon::InstanceData::Instance> _selection;
        std::vector<uint32_t> _activeIndices;

    public:
        NeuronSelector();

        NeuronSelector(NeuronScene* scene,
                       neon::ShaderUniformBuffer* uniformBuffer,
                       size_t binding);

        ~NeuronSelector() override = default;

        void setSelectionData(const Selection& selection) override;

        void clearSelection() override;

        void refreshGPUData();
    };
}

#endif //NEURONSELECTOR_H
