// Copyright (c) 2025. VG-Lab/URJC.
//
// Authors: Gael Rial Costas <gael.rial.costas@urjc.es>
//
// This file is part of Neoneuron <gitlab.gmrv.es/g.rial/neoneuron>
//
// This library is free software; you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License version 3.0 as published
// by the Free Software Foundation.
//
// This library is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this library; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

#include "ComplexGPUNeuron.h"

#include <utility>
#include <mindset/util/NeuronTransform.h>
#include <neoneuron/application/NeoneuronDefaults.h>

namespace neoneuron
{
    void ComplexGPUNeuron::generateSegmentInstances(const ComplexNeuron* neuron)
    {
        auto* instanceData = _segmentModel.lock()->getInstanceData(_segmentInstanceDataIndex);
        for (auto& segments = neuron->getSegments(); auto& segment : segments) {
            if (auto result = instanceData->createInstance(); result.isOk()) {
                _segmentInstances.push_back(result.getResult());
                _segmentInstancesByUID[segment.getUID()] = result.getResult();
            } else {
                neon::error() << "Cannot create new instances!";
                break;
            }
        }
    }

    void ComplexGPUNeuron::generateJointInstances(const ComplexNeuron* neuron)
    {
        auto* instanceData = _jointModel.lock()->getInstanceData(_jointInstanceDataIndex);
        for (auto& joints = neuron->getJoints(); auto& joint : joints) {
            if (auto result = instanceData->createInstance(); result.isOk()) {
                _jointInstances.push_back(result.getResult());
                _jointInstancesByUID[joint.getUID()] = result.getResult();
            } else {
                neon::error() << "Cannot create new instances!";
                break;
            }
        }
    }

    void ComplexGPUNeuron::generateSomaInstances(const ComplexNeuron* neuron)
    {
        auto* instanceData = _somaModel.lock()->getInstanceData(_somaInstanceDataIndex);
        for (auto& somas = neuron->getSomas(); auto& soma : somas) {
            if (auto result = instanceData->createInstance(); result.isOk()) {
                _somaInstances.push_back(result.getResult());
                _somaInstancesByUID[soma.getUID()] = result.getResult();
            } else {
                neon::error() << "Cannot create new instances!";
                break;
            }
        }
    }

    void ComplexGPUNeuron::refreshGlobalData(const ComplexNeuron* neuron, uint32_t frame) const
    {
        auto buffer = _globalInstanceData.lock();
        if (buffer == nullptr) {
            return;
        }

        mindset::UID maxSegment = 0;
        for (auto& segment : neuron->getSegments()) {
            maxSegment = std::max(maxSegment, segment.getUID());
        }

        ComplexGPUNeuronGlobalData data{.datasetId = neuron->getGID().datasetId,
                                        .neuronId = neuron->getGID().internalId,
                                        .lodMode = 8,
                                        .updateFrame = frame,
                                        .segmentsAmount = maxSegment,
                                        .radius = neuron->getBoundingBox().radius.length(),
                                        .model = rush::Mat4f(1.0f),
                                        .normal = rush::Mat4f(1.0f)};

        auto* prototype = neuron->getPrototypeNeuron();
        auto* dataset = neuron->getDataset();
        if (prototype != nullptr && dataset != nullptr) {
            if (auto prop = dataset->getProperties().getPropertyUID(mindset::PROPERTY_TRANSFORM); prop.has_value()) {
                auto transform = prototype->getProperty<mindset::NeuronTransform>(prop.value());
                if (transform.has_value()) {
                    data.model = transform->getModel();
                    data.normal = transform->getNormal();
                }
            }

            if (auto lodProp = dataset->getProperties().getPropertyUID(PROPERTY_LOD); lodProp.has_value()) {
                if (auto lod = prototype->getProperty<uint32_t>(lodProp.value()); lod.has_value()) {
                    data.lodMode = *lod;
                }
            }
        }

        buffer->uploadData(_globalInstance, 0, data);
    }

    void ComplexGPUNeuron::refreshSegments(const ComplexNeuron* neuron) const
    {
        auto neuronModel = _segmentModel.lock();
        if (neuronModel == nullptr) {
            return;
        }
        auto* neuronData = neuronModel->getInstanceData(_segmentInstanceDataIndex);
        for (size_t i = 0; i < _segmentInstances.size(); ++i) {
            auto& segment = neuron->getSegments()[i];

            uint32_t parentIndex;
            if (segment.getParentId().has_value()) {
                auto optional = neuron->findSegmentIndex(segment.getParentId().value());
                size_t index = optional.value_or(i);
                parentIndex = *_segmentInstances[index].id;
            } else {
                parentIndex = *_segmentInstances[i].id;
            }

            auto joint = neuron->findJoint(segment.getUID());

            uint32_t metadata = static_cast<uint32_t>(segment.getType()) & 0b11111111;
            ;
            metadata += (segment.getChildrenAmount() & 0b111) << 8;
            metadata += (segment.getLod() & 0b111) << 11;
            metadata += 7 << 14;

            ComplexGPUNeuronSegment gpu(*_globalInstance.id, segment.getUID(), metadata, parentIndex,
                                        rush::Vec4f(segment.getEnd(), segment.getEndRadius()));

            neuronData->uploadData(_segmentInstances[i], 0, gpu);
        }
    }

    void ComplexGPUNeuron::refreshJoints(const ComplexNeuron* neuron) const
    {
        auto jointModel = _jointModel.lock();
        if (jointModel == nullptr) {
            return;
        }
        auto* jointData = jointModel->getInstanceData(_jointInstanceDataIndex);

        for (size_t i = 0; i < _jointInstances.size(); ++i) {
            auto& joint = neuron->getJoints()[i];

            uint32_t parentInstance = *_segmentInstances[neuron->findSegmentIndex(joint.getUID()).value()].id;

            ComplexGPUNeuronJoint gpu{parentInstance, std::min(static_cast<uint32_t>(joint.getChildren().size()), 8u),
                                      joint.getRotationIndex()};

            for (size_t c = 0; c < gpu.amount; ++c) {
                gpu.connections[c] = *_segmentInstances[neuron->findSegmentIndex(joint.getChildren()[c]).value()].id;
            }

            jointData->uploadData(_jointInstances[i], 0, gpu);
        }
    }

    void ComplexGPUNeuron::refreshSomas(const ComplexNeuron* neuron) const
    {
        auto somaModel = _somaModel.lock();
        if (somaModel == nullptr) {
            return;
        }
        auto* somaData = somaModel->getInstanceData(_somaInstanceDataIndex);

        for (size_t i = 0; i < _somaInstances.size(); ++i) {
            auto& soma = neuron->getSomas()[i];
            uint32_t segment = *_segmentInstances[neuron->findSegmentIndex(soma.getUID()).value()].id;

            ComplexGPUNeuronSoma gpu{
                segment,
                std::min(static_cast<uint32_t>(soma.getChildren().size()), 15u),
            };

            for (size_t c = 0; c < gpu.amount; ++c) {
                gpu.connections[c] = *_segmentInstances[neuron->findSegmentIndex(soma.getChildren()[c]).value()].id;
            }

            somaData->uploadData(_somaInstances[i], 0, gpu);
        }
    }

    void ComplexGPUNeuron::invalidate()
    {
        if (!_valid) {
            return;
        }
        _valid = false;

        if (auto buf = _globalInstanceData.lock(); buf != nullptr) {
            buf->freeInstance(_globalInstance);
        }

        if (auto model = _segmentModel.lock(); model != nullptr) {
            auto* segmentData = model->getInstanceData(_segmentInstanceDataIndex);
            for (auto instance : _segmentInstances) {
                segmentData->freeInstance(instance);
            }
        }

        if (auto model = _jointModel.lock(); model != nullptr) {
            auto* jointData = model->getInstanceData(_jointInstanceDataIndex);
            for (auto instance : _jointInstances) {
                jointData->freeInstance(instance);
            }
        }

        if (auto model = _somaModel.lock(); model != nullptr) {
            auto* somaData = model->getInstanceData(_somaInstanceDataIndex);
            for (auto instance : _somaInstances) {
                somaData->freeInstance(instance);
            }
        }
    }

    ComplexGPUNeuron::ComplexGPUNeuron(ComplexGPUNeuron&& other) noexcept
    {
        if (this == &other) {
            return;
        }
        _globalInstanceData = std::move(other._globalInstanceData);
        _segmentModel = std::move(other._segmentModel);
        _jointModel = std::move(other._jointModel);
        _somaModel = std::move(other._somaModel);
        _segmentInstanceDataIndex = other._segmentInstanceDataIndex;
        _jointInstanceDataIndex = other._jointInstanceDataIndex;
        _somaInstanceDataIndex = other._somaInstanceDataIndex;
        _globalInstance = other._globalInstance;
        _segmentInstances = std::move(other._segmentInstances);
        _segmentInstancesByUID = std::move(other._segmentInstancesByUID);
        _jointInstances = std::move(other._jointInstances);
        _jointInstancesByUID = std::move(other._jointInstancesByUID);
        _somaInstances = std::move(other._somaInstances);
        _somaInstancesByUID = std::move(other._somaInstancesByUID);
        _valid = other._valid;
        other._valid = false;
    }

    ComplexGPUNeuron::ComplexGPUNeuron(std::weak_ptr<neon::InstanceData> globalInstanceData,
                                       std::weak_ptr<neon::Model> neuronModel, std::weak_ptr<neon::Model> jointModel,
                                       std::weak_ptr<neon::Model> somaModel, size_t segmentInstanceDataIndex,
                                       size_t jointInstanceDataIndex, size_t somaInstanceDataIndex,
                                       const ComplexNeuron* neuron, uint32_t frame) :
        _globalInstanceData(std::move(globalInstanceData)),
        _segmentModel(std::move(neuronModel)),
        _jointModel(std::move(jointModel)),
        _somaModel(std::move(somaModel)),
        _segmentInstanceDataIndex(segmentInstanceDataIndex),
        _jointInstanceDataIndex(jointInstanceDataIndex),
        _somaInstanceDataIndex(somaInstanceDataIndex),
        _valid(true)
    {
        _segmentInstances.reserve(neuron->getSegments().size());

        _globalInstance = _globalInstanceData.lock()->createInstance().orElse(_globalInstance);
        generateSegmentInstances(neuron);
        generateJointInstances(neuron);
        generateSomaInstances(neuron);

        refreshGPUData(neuron, frame);
    }

    ComplexGPUNeuron::~ComplexGPUNeuron()
    {
        invalidate();
    }

    void ComplexGPUNeuron::refreshGPUData(const ComplexNeuron* neuron, uint32_t frame) const
    {
        if (!_valid) {
            return;
        }
        refreshGlobalData(neuron, frame);
        refreshSegments(neuron);
        refreshJoints(neuron);
        refreshSomas(neuron);
    }

    void ComplexGPUNeuron::refreshProperty(const ComplexNeuron* neuron, uint32_t frame, const std::string& propertyName)
    {
        if (propertyName == mindset::PROPERTY_TRANSFORM) {
            refreshGlobalData(neuron, frame);
        } else if (propertyName == PROPERTY_LOD) {
            refreshGlobalData(neuron, frame);
        }
    }

    std::optional<neon::InstanceData::Instance> ComplexGPUNeuron::findSegment(mindset::UID uid) const
    {
        auto id = _segmentInstancesByUID.find(uid);
        if (id == _segmentInstancesByUID.end()) {
            return {};
        }
        return id->second;
    }

    std::optional<neon::InstanceData::Instance> ComplexGPUNeuron::findJoint(mindset::UID uid) const
    {
        auto id = _jointInstancesByUID.find(uid);
        if (id == _segmentInstancesByUID.end()) {
            return {};
        }
        return id->second;
    }

    ComplexGPUNeuron& ComplexGPUNeuron::operator=(ComplexGPUNeuron&& other) noexcept
    {
        if (this == &other) {
            return *this;
        }

        // Delete the data of the current neuron.
        invalidate();

        _globalInstanceData = std::move(other._globalInstanceData);
        _segmentModel = std::move(other._segmentModel);
        _jointModel = std::move(other._jointModel);
        _somaModel = std::move(other._somaModel);
        _segmentInstanceDataIndex = other._segmentInstanceDataIndex;
        _jointInstanceDataIndex = other._jointInstanceDataIndex;
        _somaInstanceDataIndex = other._somaInstanceDataIndex;
        _globalInstance = other._globalInstance;
        _segmentInstances = std::move(other._segmentInstances);
        _segmentInstancesByUID = std::move(other._segmentInstancesByUID);
        _jointInstances = std::move(other._jointInstances);
        _jointInstancesByUID = std::move(other._jointInstancesByUID);
        _somaInstances = std::move(other._somaInstances);
        _somaInstancesByUID = std::move(other._somaInstancesByUID);
        _valid = other._valid;
        other._valid = false;
        return *this;
    }
} // namespace neoneuron
