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

    void ComplexNeuronSelector::sendCenterToCamera() const {
        if (_centerAccumulatorAmount != 0) {
            auto center = _centerAccumulator / _centerAccumulatorAmount;
            auto c = _scene->getRender()->getCameraData().getCameraController();
            if (auto orb = dynamic_cast<OrbitalCameraController*>(c.raw())) {
                orb->setCenterKeepPosition(center);
            }
        }
    }

    ComplexNeuronSelector::ComplexNeuronSelector()
        : _scene(nullptr),
          _uniformBuffer(nullptr),
          _binding(0),
          _centerAccumulatorAmount(0) {}

    ComplexNeuronSelector::ComplexNeuronSelector(ComplexNeuronScene* scene,
                                                 neon::ShaderUniformBuffer* uniformBuffer,
                                                 size_t binding)
        : _scene(scene),
          _uniformBuffer(uniformBuffer),
          _binding(binding),
          _centerAccumulatorAmount(0) {}

    const std::unordered_set<UID> ComplexNeuronSelector::getSelectedNeurons() {
        return _selectedNeurons;
    }

    void ComplexNeuronSelector::setSelectionData(const Selection& selection) {
        if (selection.clear) {
            clearSelection();
        }

        std::unordered_map<UID, rush::Mat4f> transforms;

        for (auto& any: selection.selections) {
            if (any.type() != typeid(rush::Vec<2, UID>)) continue;
            auto pair = std::any_cast<rush::Vec<2, UID>>(any);

            neon::Logger::defaultLogger()->debug(neon::MessageBuilder().print("Selected: ").print(pair));

            UID neuronId = pair[0];
            UID sectionId = pair[1];
            selectSection(neuronId, sectionId, &transforms);
        }

        sendCenterToCamera();
    }

    void ComplexNeuronSelector::clearSelection() {
        _selection.clear();
        _selectedNeurons.clear();
        refreshGPUData();

        _centerAccumulator = rush::Vec3f();
        _centerAccumulatorAmount = 0;
    }

    void ComplexNeuronSelector::selectNeuron(UID neuronId) {
        auto* buffer = static_cast<ComplexGPUNeuronSelectionData*>(_uniformBuffer->fetchData(_binding));

        auto neuron = _scene->findGPUNeuron(neuronId);
        if (!neuron.has_value()) return;
        auto realNeuron = _scene->findNeuron(neuronId);
        if (!realNeuron.has_value()) return;

        rush::Mat4f transform = fetchTransform(*realNeuron.value());

        for (auto& segment: realNeuron.value()->getSegments()) {
            if (auto gpuSegment = neuron.value()->findSegment(segment.getId()); gpuSegment.has_value()) {
                ComplexSelectionEntry entry{neuronId, segment.getId()};
                if (_selection.contains(entry)) continue;
                uint32_t id = *gpuSegment.value().id;
                _selection[entry] = gpuSegment.value();
                _activeIndices.push_back(id);
                (buffer + id)->selected = true;

                _centerAccumulator += rush::Vec3f(transform * rush::Vec4f(segment.getEnd(), 1.0f));
                ++_centerAccumulatorAmount;
            }
        }

        _selectedNeurons.insert(neuronId);
    }

    void ComplexNeuronSelector::selectSection(UID neuronId, UID sectionId) {
        selectSection(neuronId, sectionId, nullptr);
    }

    void ComplexNeuronSelector::selectSection(UID neuronId, UID sectionId,
                                              std::unordered_map<UID, rush::Mat4f>* transforms) {
        auto* buffer = static_cast<ComplexGPUNeuronSelectionData*>(_uniformBuffer->fetchData(_binding));

        ComplexSelectionEntry entry{neuronId, sectionId};
        if (_selection.contains(entry)) return;

        auto neuron = _scene->findGPUNeuron(neuronId);
        if (!neuron.has_value()) return;
        auto segment = neuron.value()->findSegment(sectionId);
        if (!segment.has_value()) return;


        uint32_t id = *segment.value().id;
        _selection[entry] = segment.value();
        _selectedNeurons.insert(neuronId);
        _activeIndices.push_back(id);
        (buffer + id)->selected = true;

        // Find segment data
        auto realNeuron = _scene->findNeuron(neuronId);
        if (!realNeuron.has_value()) return;
        auto realSegment = realNeuron.value()->findSegment(sectionId);
        if (!realSegment.has_value()) return;

        rush::Mat4f transform;
        if (transforms != nullptr) {
            if (auto it = transforms->find(neuronId); it == transforms->end()) {
                transform = fetchTransform(*realNeuron.value());
                transforms->insert({neuronId, transform});
            } else {
                transform = it->second;
            }
        }

        _centerAccumulator += rush::Vec3f(transform * rush::Vec4f(realSegment.value()->getEnd(), 1.0f));
        ++_centerAccumulatorAmount;
    }

    void ComplexNeuronSelector::deselectNeuron(UID neuronId) {
        _selectedNeurons.erase(neuronId);
        size_t amount = std::erase_if(_selection, [neuronId](const auto& entry) {
            return entry.first.neuron == neuronId;
        });
        if (amount > 0) {
            refreshGPUData();
        }
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
