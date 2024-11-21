//
// Created by gaeqs on 15/11/24.
//

#include "SimpleNeuronSelector.h"

#include "SimpleNeuronScene.h"

namespace neoneuron {
    bool SimpleSelectionEntry::operator==(const SimpleSelectionEntry& other) const {
        return neuron == other.neuron && segment == other.segment;
    }

    bool SimpleSelectionEntry::operator!=(const SimpleSelectionEntry& other) const {
        return neuron != other.neuron || segment != other.segment;
    }

    SimpleNeuronSelector::SimpleNeuronSelector() : _scene(nullptr), _uniformBuffer(nullptr), _binding(0) {}

    SimpleNeuronSelector::SimpleNeuronSelector(SimpleNeuronScene* scene,
                                               neon::ShaderUniformBuffer* uniformBuffer,
                                               size_t binding)
        : _scene(scene),
          _uniformBuffer(uniformBuffer),
          _binding(binding) {}

    void SimpleNeuronSelector::setSelectionData(const Selection& selection) {
        auto* buffer = static_cast<GPUNeuronSelectionData*>(_uniformBuffer->fetchData(_binding));

        if (selection.clear) {
            for (uint32_t id: _activeIndices) {
                (buffer + id)->selected = false;
            }
            _selection.clear();
            _activeIndices.clear();
        }

        for (auto& any: selection.selections) {
            if (any.type() != typeid(rush::Vec<2, UID>)) continue;
            auto pair = std::any_cast<rush::Vec<2, UID>>(any);

            UID neuronId = pair[0];
            UID sectionId = pair[1];

            SimpleSelectionEntry entry{neuronId, sectionId};
            if (_selection.contains(entry)) continue;

            auto neuron = _scene->findGPUNeuron(neuronId);
            if (!neuron.has_value()) continue;
            auto segment = neuron.value()->findSegment(sectionId);
            if (!segment.has_value()) continue;


            uint32_t id = *segment.value().id;
            _selection[entry] = segment.value();
            _activeIndices.push_back(id);
            (buffer + id)->selected = true;
        }
    }

    void SimpleNeuronSelector::clearSelection() {
        _selection.clear();
        refreshGPUData();
    }

    void SimpleNeuronSelector::refreshGPUData() {
        auto data = static_cast<GPUNeuronSelectionData*>(_uniformBuffer->fetchData(_binding));

        for (uint32_t id: _activeIndices) {
            (data + id)->selected = false;
        }

        _activeIndices.clear();

        for (auto& [id]: _selection | std::views::values) {
            (data + *id)->selected = true;
            _activeIndices.push_back(*id);
        }
    }
}
