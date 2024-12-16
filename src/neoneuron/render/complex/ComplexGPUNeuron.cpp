//
// Created by gaeqs on 10/10/24.
//

#include "ComplexGPUNeuron.h"

#include <utility>


namespace neoneuron {
    void ComplexGPUNeuron::generateSegmentInstances() {
        auto* instanceData = _segmentModel.lock()->getInstanceData(_segmentInstanceDataIndex);
        for (auto& segments = _neuron->getSegments(); auto& segment: segments) {
            if (auto result = instanceData->createInstance(); result.isOk()) {
                _segmentInstances.push_back(result.getResult());
                _segmentInstancesByUID[segment.getId()] = result.getResult();
            } else {
                neon::Logger::defaultLogger()->error("Cannot create new instances!");
                break;
            }
        }
    }

    void ComplexGPUNeuron::generateJointInstances() {
        auto* instanceData = _jointModel.lock()->getInstanceData(_jointInstanceDataIndex);
        for (auto& joints = _neuron->getJoints(); auto& joint: joints) {
            if (auto result = instanceData->createInstance(); result.isOk()) {
                _jointInstances.push_back(result.getResult());
                _jointInstancesByUID[joint.getId()] = result.getResult();
            } else {
                neon::Logger::defaultLogger()->error("Cannot create new instances!");
                break;
            }
        }
    }

    void ComplexGPUNeuron::generateSomaInstances() {
        auto* instanceData = _somaModel.lock()->getInstanceData(_somaInstanceDataIndex);
        for (auto& somas = _neuron->getSomas(); auto& soma: somas) {
            if (auto result = instanceData->createInstance(); result.isOk()) {
                _somaInstances.push_back(result.getResult());
                _somaInstancesByUID[soma.getId()] = result.getResult();
            } else {
                neon::Logger::defaultLogger()->error("Cannot create new instances!");
                break;
            }
        }
    }

    void ComplexGPUNeuron::refreshSegments() const {
        auto neuronModel = _segmentModel.lock();
        if (neuronModel == nullptr) return;
        auto* neuronData = neuronModel->getInstanceData(_segmentInstanceDataIndex);
        for (size_t i = 0; i < _segmentInstances.size(); ++i) {
            auto& segment = _neuron->getSegments()[i];

            uint32_t parentIndex;
            if (segment.getParentId().has_value()) {
                size_t index = _neuron->findSegmentIndex(segment.getParentId().value()).value();
                parentIndex = *_segmentInstances[index].id;
            } else {
                parentIndex = *_segmentInstances[i].id;
            }

            auto joint = _neuron->findJoint(segment.getId());

            uint32_t metadata = static_cast<uint32_t>(segment.getType()) & 0b11111111;;
            if (joint.has_value()) {
                metadata += (static_cast<uint32_t>(joint.value()->getChildren().size()) & 0b111) << 8;
            }

            ComplexGPUNeuronSegment gpu(
                _neuron->getId(),
                segment.getId(),
                metadata,
                parentIndex,
                rush::Vec4f(segment.getEnd(), segment.getEndRadius())
            );

            neuronData->uploadData(_segmentInstances[i], 0, gpu);
        }
    }

    void ComplexGPUNeuron::refreshJoints() const {
        auto jointModel = _jointModel.lock();
        if (jointModel == nullptr) return;
        auto* jointData = jointModel->getInstanceData(_jointInstanceDataIndex);

        for (size_t i = 0; i < _jointInstances.size(); ++i) {
            auto& joint = _neuron->getJoints()[i];

            uint32_t parentInstance = *_segmentInstances[_neuron->findSegmentIndex(joint.getId()).value()].id;

            ComplexGPUNeuronJoint gpu{
                parentInstance,
                std::min(static_cast<uint32_t>(joint.getChildren().size()), 8u),
                joint.getRotationIndex()
            };

            for (size_t c = 0; c < gpu.amount; ++c) {
                gpu.connections[c] = *_segmentInstances[_neuron->findSegmentIndex(joint.getChildren()[c]).value()].id;
            }

            jointData->uploadData(_jointInstances[i], 0, gpu);
        }
    }

    void ComplexGPUNeuron::refreshSomas() const {
        auto somaModel = _somaModel.lock();
        if (somaModel == nullptr) return;
        auto* somaData = somaModel->getInstanceData(_somaInstanceDataIndex);

        for (size_t i = 0; i < _somaInstances.size(); ++i) {
            auto& soma = _neuron->getSomas()[i];
            uint32_t segment = *_segmentInstances[_neuron->findSegmentIndex(soma.getId()).value()].id;

            ComplexGPUNeuronSoma gpu{
                segment,
                std::min(static_cast<uint32_t>(soma.getChildren().size()), 8u),
            };

            for (size_t c = 0; c < gpu.amount; ++c) {
                gpu.connections[c] = *_segmentInstances[_neuron->findSegmentIndex(soma.getChildren()[c]).value()].id;
            }

            somaData->uploadData(_somaInstances[i], 0, gpu);
        }
    }

    ComplexGPUNeuron::ComplexGPUNeuron(ComplexGPUNeuron&& other) noexcept {
        if (this == &other) return;
        _segmentModel = std::move(other._segmentModel);
        _jointModel = std::move(other._jointModel);
        _somaModel = std::move(other._somaModel);
        _segmentInstanceDataIndex = other._segmentInstanceDataIndex;
        _jointInstanceDataIndex = other._jointInstanceDataIndex;
        _somaInstanceDataIndex = other._somaInstanceDataIndex;
        _segmentInstances = std::move(other._segmentInstances);
        _segmentInstancesByUID = std::move(other._segmentInstancesByUID);
        _jointInstances = std::move(other._jointInstances);
        _jointInstancesByUID = std::move(other._jointInstancesByUID);
        _somaInstances = std::move(other._somaInstances);
        _somaInstancesByUID = std::move(other._somaInstancesByUID);
        _neuron = other._neuron;
        _valid = other._valid;
        other._valid = false;
    }

    ComplexGPUNeuron::ComplexGPUNeuron(std::weak_ptr<neon::Model> neuronModel,
                                       std::weak_ptr<neon::Model> jointModel,
                                       std::weak_ptr<neon::Model> somaModel,
                                       size_t segmentInstanceDataIndex,
                                       size_t jointInstanceDataIndex,
                                       size_t somaInstanceDataIndex,
                                       const ComplexNeuron* neuron)
        : _segmentModel(std::move(neuronModel)),
          _jointModel(std::move(jointModel)),
          _somaModel(std::move(somaModel)),
          _segmentInstanceDataIndex(segmentInstanceDataIndex),
          _jointInstanceDataIndex(jointInstanceDataIndex),
          _somaInstanceDataIndex(somaInstanceDataIndex),
          _neuron(neuron),
          _valid(true) {
        _segmentInstances.reserve(_neuron->getSegments().size());

        generateSegmentInstances();
        generateJointInstances();
        generateSomaInstances();

        refreshGPUData();
    }

    ComplexGPUNeuron::~ComplexGPUNeuron() {
        if (!_valid) return;
        _valid = false;

        if (auto model = _segmentModel.lock(); model != nullptr) {
            auto* segmentData = model->getInstanceData(_segmentInstanceDataIndex);
            for (auto instance: _segmentInstances) {
                segmentData->freeInstance(instance);
            }
        }

        if (auto model = _jointModel.lock(); model != nullptr) {
            auto* jointData = model->getInstanceData(_jointInstanceDataIndex);
            for (auto instance: _jointInstances) {
                jointData->freeInstance(instance);
            }
        }

        if (auto model = _somaModel.lock(); model != nullptr) {
            auto* somaData = model->getInstanceData(_somaInstanceDataIndex);
            for (auto instance: _somaInstances) {
                somaData->freeInstance(instance);
            }
        }
    }

    void ComplexGPUNeuron::refreshGPUData() const {
        if (!_valid) return;
        refreshSegments();
        refreshJoints();
        refreshSomas();
    }

    std::optional<neon::InstanceData::Instance> ComplexGPUNeuron::findSegment(UID uid) const {
        auto id = _segmentInstancesByUID.find(uid);
        if (id == _segmentInstancesByUID.end()) return {};
        return id->second;
    }

    std::optional<neon::InstanceData::Instance> ComplexGPUNeuron::findJoint(UID uid) const {
        auto id = _jointInstancesByUID.find(uid);
        if (id == _segmentInstancesByUID.end()) return {};
        return id->second;
    }

    ComplexGPUNeuron& ComplexGPUNeuron::operator=(ComplexGPUNeuron&& other) noexcept {
        if (this == &other) return *this;
        _segmentModel = std::move(other._segmentModel);
        _jointModel = std::move(other._jointModel);
        _segmentInstanceDataIndex = other._segmentInstanceDataIndex;
        _jointInstanceDataIndex = other._jointInstanceDataIndex;
        _segmentInstances = std::move(other._segmentInstances);
        _segmentInstancesByUID = std::move(other._segmentInstancesByUID);
        _jointInstances = std::move(other._jointInstances);
        _jointInstancesByUID = std::move(other._jointInstancesByUID);
        _neuron = other._neuron;
        _valid = other._valid;
        other._valid = false;
        return *this;
    }
}
