//
// Created by gaeqs on 15/11/24.
//

#include "ComplexNeuronSelector.h"

#include <neoneuron/render/NeoneuronRender.h>
#include <neoneuron/render/component/camera/OrbitalCameraController.h>

#include "ComplexNeuronScene.h"

namespace neoneuron {
    namespace {
        rush::Mat4f fetchTransform(const ComplexNeuron& neuron) {
            if (auto prototype = neuron.getPrototypeNeuron(); prototype.value()) {
                auto transform = prototype.value()->getProperty<NeuronTransform>(PROPERTY_TRANSFORM);
                if (transform.has_value()) {
                    return transform.value().getModel();
                }
            }
            return rush::Mat4f(1.0f);
        }
    }

    bool ComplexSelectionEntry::operator==(const ComplexSelectionEntry& other) const {
        return neuron == other.neuron && segment == other.segment;
    }

    bool ComplexSelectionEntry::operator!=(const ComplexSelectionEntry& other) const {
        return neuron != other.neuron || segment != other.segment;
    }

    ComplexNeuronSelector::ComplexNeuronSelector() : _scene(nullptr), _uniformBuffer(nullptr), _binding(0) {}

    ComplexNeuronSelector::ComplexNeuronSelector(ComplexNeuronScene* scene,
                                                 neon::ShaderUniformBuffer* uniformBuffer,
                                                 size_t binding)
        : _scene(scene),
          _uniformBuffer(uniformBuffer),
          _binding(binding) {}

    void ComplexNeuronSelector::setSelectionData(const Selection& selection) {
        auto* buffer = static_cast<ComplexGPUNeuronSelectionData*>(_uniformBuffer->fetchData(_binding));

        if (selection.clear) {
            for (uint32_t id: _activeIndices) {
                (buffer + id)->selected = false;
            }
            _selection.clear();
            _activeIndices.clear();
        }

        rush::Vec3f center;
        size_t centerAmount = 0;

        std::unordered_map<UID, rush::Mat4f> transforms;

        for (auto& any: selection.selections) {
            if (any.type() != typeid(rush::Vec<2, UID>)) continue;
            auto pair = std::any_cast<rush::Vec<2, UID>>(any);

            neon::Logger::defaultLogger()->debug(neon::MessageBuilder().print("Selected: ").print(pair));

            UID neuronId = pair[0];
            UID sectionId = pair[1];

            ComplexSelectionEntry entry{neuronId, sectionId};
            if (_selection.contains(entry)) continue;

            auto neuron = _scene->findGPUNeuron(neuronId);
            if (!neuron.has_value()) continue;
            auto segment = neuron.value()->findSegment(sectionId);
            if (!segment.has_value()) continue;


            uint32_t id = *segment.value().id;
            _selection[entry] = segment.value();
            _activeIndices.push_back(id);
            (buffer + id)->selected = true;

            // Find segment data
            auto realNeuron = _scene->findNeuron(neuronId);
            if (!realNeuron.has_value()) continue;
            auto realSegment = realNeuron.value()->findSegment(sectionId);
            if (!realSegment.has_value()) continue;

            auto it = transforms.find(neuronId);
            rush::Mat4f transform;
            if (it == transforms.end()) {
                transform = fetchTransform(*realNeuron.value());
                transforms.insert({neuronId, transform});
            } else {
                transform = it->second;
            }

            center += rush::Vec3f(transform * rush::Vec4f(realSegment.value()->getEnd(), 1.0f));
            ++centerAmount;
        }

        if (centerAmount != 0) {
            center /= centerAmount;
            auto c = _scene->getRender()->getCameraData().getCameraController();
            if (auto orb = dynamic_cast<OrbitalCameraController*>(c.raw())) {
                orb->setCenterKeepPosition(center);
            }
        }
    }

    void ComplexNeuronSelector::clearSelection() {
        _selection.clear();
        refreshGPUData();
    }

    void ComplexNeuronSelector::refreshGPUData() {
        auto data = static_cast<ComplexGPUNeuronSelectionData*>(_uniformBuffer->fetchData(_binding));

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
