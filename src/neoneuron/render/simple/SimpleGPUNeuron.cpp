//
// Created by gaeqs on 10/10/24.
//

#include "SimpleGPUNeuron.h"

#include <utility>


namespace neoneuron {
    SimpleGPUNeuron::SimpleGPUNeuron(SimpleGPUNeuron&& other) noexcept {
        _model = other._model;
        _instanceDataIndex = other._instanceDataIndex;
        _instances = std::move(other._instances);
        _neuron = other._neuron;
        _valid = other._valid;
        other._valid = false;
    }

    SimpleGPUNeuron::SimpleGPUNeuron(std::weak_ptr<neon::Model> model,
                         size_t instanceDataIndex,
                         const SimpleNeuron* neuron)
        : _model(std::move(model)),
          _instanceDataIndex(instanceDataIndex),
          _neuron(neuron),
          _valid(true) {
        _instances.reserve(_neuron->getSegments().size());

        auto* instanceData = _model.lock()->getInstanceData(_instanceDataIndex);

        auto& segments = _neuron->getSegments();
        for (auto& segment: segments) {
            auto result = instanceData->createInstance();
            if (result.isOk()) {
                _instances.push_back(result.getResult());
                _instancesByUID[segment.getId()] = result.getResult();
            } else {
                neon::error() << "Cannot create new instances!";
                break;
            }
        }

        refreshGPUData();
    }

    SimpleGPUNeuron::~SimpleGPUNeuron() {
        if (!_valid) return;
        _valid = false;
        auto model = _model.lock();
        if (model == nullptr) return;
        auto* instanceData = model->getInstanceData(_instanceDataIndex);
        for (auto instance: _instances) {
            instanceData->freeInstance(instance);
        }
    }

    void SimpleGPUNeuron::refreshGPUData() const {
        if (!_valid) return;
        auto model = _model.lock();
        if (model == nullptr) return;
        auto* instanceData = model->getInstanceData(_instanceDataIndex);
        for (size_t i = 0; i < _instances.size(); ++i) {
            auto& segment = _neuron->getSegments()[i];

            uint32_t parentIndex;
            if (segment.getParentId().has_value()) {
                size_t index = _neuron->findSegmentIndex(segment.getParentId().value()).value();
                parentIndex = *_instances[index].id;
            } else {
                parentIndex = *_instances[i].id;
            }

            SimpleGPUNeuronSegment gpu(
                _neuron->getId(),
                segment.getId(),
                static_cast<uint32_t>(segment.getType()),
                parentIndex,
                rush::Vec4f(segment.getEnd(), segment.getEndRadius())
            );

            instanceData->uploadData(_instances[i], 0, gpu);
        }
    }

    std::optional<neon::InstanceData::Instance> SimpleGPUNeuron::findSegment(UID uid) const {
        auto id = _instancesByUID.find(uid);
        if (id == _instancesByUID.end()) return {};
        return id->second;
    }

    SimpleGPUNeuron& SimpleGPUNeuron::operator=(SimpleGPUNeuron&& other) noexcept {
        if (this == &other) return *this;
        _model = other._model;
        _instanceDataIndex = other._instanceDataIndex;
        _instances = std::move(other._instances);
        _neuron = other._neuron;
        return *this;
    }
}
