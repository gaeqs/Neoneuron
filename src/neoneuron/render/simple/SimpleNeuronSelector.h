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
    struct SimpleSelectionEntry {
        UID neuron;
        UID segment;

        bool operator==(const SimpleSelectionEntry& other) const;

        bool operator!=(const SimpleSelectionEntry& other) const;
    };
}

template<>
struct std::hash<neoneuron::SimpleSelectionEntry> {
    size_t operator()(const neoneuron::SimpleSelectionEntry& entry) const noexcept {
        std::hash<uint32_t> hasher;
        size_t hash1 = hasher(entry.neuron);
        size_t hash2 = hasher(entry.segment);
        return hash1 ^ hash2 + 0x9e3779b9 + (hash1 << 6) + (hash1 >> 2);
    }
};

namespace neoneuron {
    class SimpleNeuronScene;

    struct SimpleGPUNeuronSelectionData {
        uint32_t selected;
    };

    class SimpleNeuronSelector : public AbstractSelector {
        SimpleNeuronScene* _scene;
        neon::ShaderUniformBuffer* _uniformBuffer;
        size_t _binding;

        std::unordered_map<SimpleSelectionEntry, neon::InstanceData::Instance> _selection;
        std::vector<uint32_t> _activeIndices;

    public:
        SimpleNeuronSelector();

        SimpleNeuronSelector(SimpleNeuronScene* scene,
                       neon::ShaderUniformBuffer* uniformBuffer,
                       size_t binding);

        ~SimpleNeuronSelector() override = default;

        void setSelectionData(const Selection& selection) override;

        void clearSelection() override;

        void refreshGPUData();
    };
}

#endif //NEURONSELECTOR_H
