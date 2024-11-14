//
// Created by gaeqs on 10/10/24.
//

#include "GPUNeuron.h"

#include <utility>


namespace neoneuron {
    GPUNeuron::GPUNeuron(GPUNeuron&& other) noexcept {
        _model = other._model;
        _instanceDataIndex = other._instanceDataIndex;
        _instances = std::move(other._instances);
        _neuron = other._neuron;
        _valid = other._valid;
        other._valid = false;
    }

    GPUNeuron::GPUNeuron(std::weak_ptr<neon::Model> model,
                         size_t instanceDataIndex,
                         const Neuron* neuron)
        : _model(std::move(model)),
          _instanceDataIndex(instanceDataIndex),
          _neuron(neuron),
          _valid(true) {
        _instances.reserve(_neuron->getSegments().size());

        auto* instanceData = _model.lock()->getInstanceData(_instanceDataIndex);

        for (size_t i = 0; i < _neuron->getSegments().size(); ++i) {
            auto result = instanceData->createInstance();
            if (result.isOk()) {
                _instances.push_back(result.getResult());
            } else {
                neon::Logger::defaultLogger()->error("Cannot create new instances!");
                break;
            }
        }

        refreshGPUData();
    }

    GPUNeuron::~GPUNeuron() {
        if (!_valid) return;
        _valid = false;
        auto model = _model.lock();
        if (model == nullptr) return;
        auto* instanceData = model->getInstanceData(_instanceDataIndex);
        for (auto instance: _instances) {
            instanceData->freeInstance(instance);
        }
    }

    void GPUNeuron::refreshGPUData() const {
        if (!_valid) return;
        std::unordered_map<UID, size_t> positions;
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

            GPUNeuronSegment gpu(
                _neuron->getId(),
                segment.getId(),
                static_cast<uint32_t>(segment.getType()),
                parentIndex,
                rush::Vec4f(segment.getEnd(), segment.getEndRadius())
            );

            instanceData->uploadData(_instances[i], 0, gpu);
        }
    }

    GPUNeuron& GPUNeuron::operator=(GPUNeuron&& other) noexcept {
        if (this == &other) return *this;
        _model = other._model;
        _instanceDataIndex = other._instanceDataIndex;
        _instances = std::move(other._instances);
        _neuron = other._neuron;
        return *this;
    }
}
