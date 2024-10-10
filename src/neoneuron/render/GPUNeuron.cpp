//
// Created by gaeqs on 10/10/24.
//

#include "GPUNeuron.h"


namespace neoneuron {
    GPUNeuron::GPUNeuron(GPUNeuron&& other) noexcept {
        _instanceData = other._instanceData;
        _instances = std::move(other._instances);
        _neuron = other._neuron;
    }

    GPUNeuron::GPUNeuron(neon::InstanceData* instanceData, const Neuron* neuron)
        : _instanceData(instanceData),
          _neuron(neuron) {
        _instances.reserve(_neuron->getSegments().size());
        for (size_t i = 0; i < _neuron->getSegments().size(); ++i) {
            auto result = _instanceData->createInstance();
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
        for (auto instance: _instances) {
            _instanceData->freeInstance(instance);
        }
    }

    void GPUNeuron::refreshGPUData() const {
        std::unordered_map<UID, size_t> positions;

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
                rush::Vec4f(segment.getEnd(), segment.getEndRadius()),
                static_cast<uint32_t>(segment.getType()),
                parentIndex
            );

            _instanceData->uploadData(_instances[i], 0, gpu);
        }
    }

    GPUNeuron& GPUNeuron::operator=(GPUNeuron&& other) noexcept {
        if (this == &other) return *this;
        _instanceData = other._instanceData;
        _instances = std::move(other._instances);
        _neuron = other._neuron;
        return *this;
    }
}
