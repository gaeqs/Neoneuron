//
// Created by gaeqs on 15/11/24.
//

#ifndef COMPLEXNEURONSELECTOR_H
#define COMPLEXNEURONSELECTOR_H

#include <unordered_map>
#include <neon/render/model/InstanceData.h>

#include <neon/render/shader/ShaderUniformBuffer.h>
#include <neoneuron/Types.h>
#include <neoneuron/render/AbstractSelector.h>


namespace neoneuron {
    struct ComplexSelectionEntry {
        UID neuron;
        UID segment;

        bool operator==(const ComplexSelectionEntry& other) const;

        bool operator!=(const ComplexSelectionEntry& other) const;
    };
}

template<>
struct std::hash<neoneuron::ComplexSelectionEntry> {
    size_t operator()(const neoneuron::ComplexSelectionEntry& entry) const noexcept {
        std::hash<uint32_t> hasher;
        size_t hash1 = hasher(entry.neuron);
        size_t hash2 = hasher(entry.segment);
        return hash1 ^ hash2 + 0x9e3779b9 + (hash1 << 6) + (hash1 >> 2);
    }
};

namespace neoneuron {
    class ComplexNeuronScene;

    struct ComplexGPUNeuronSelectionData {
        uint32_t selected;
    };

    class ComplexNeuronSelector : public AbstractSelector {
        ComplexNeuronScene* _scene;
        neon::ShaderUniformBuffer* _uniformBuffer;
        size_t _binding;

        std::unordered_map<ComplexSelectionEntry, neon::InstanceData::Instance> _selection;
        std::vector<uint32_t> _activeIndices;

    public:
        ComplexNeuronSelector();

        ComplexNeuronSelector(ComplexNeuronScene* scene,
                       neon::ShaderUniformBuffer* uniformBuffer,
                       size_t binding);

        ~ComplexNeuronSelector() override = default;

        void setSelectionData(const Selection& selection) override;

        void clearSelection() override;

        void refreshGPUData();
    };
}

#endif //COMPLEXNEURONSELECTOR_H
