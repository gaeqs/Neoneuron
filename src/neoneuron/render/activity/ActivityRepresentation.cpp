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

//
// Created by gaeqs on 8/10/25.
//

#include "ActivityRepresentation.h"

CMRC_DECLARE(resources);

namespace neoneuron
{

    void ActivityRepresentation::loadUniformBuffers()
    {
        auto* app = &_render->getApplication();

        std::vector bindings = {
            neon::ShaderUniformBinding::uniformBuffer(sizeof(ActivityRepresentationData)),
            neon::ShaderUniformBinding::uniformBuffer(sizeof(ActivityRepresentationVolatileData)),
            neon::ShaderUniformBinding::storageBuffer(sizeof(ActivityGPUNeuronData) * ACTIVITY_INSTANCES),
            neon::ShaderUniformBinding::storageBuffer(sizeof(float) * ACTIVITY_INSTANCES),
        };

        _uboDescriptor =
            std::make_shared<neon::ShaderUniformDescriptor>(app, "neoneuron:activity_descriptor", bindings);

        _ubo = std::make_shared<neon::ShaderUniformBuffer>("neoneuron:activity_ubo", _uboDescriptor);
    }

    void ActivityRepresentation::loadShader()
    {
        auto shader = std::make_shared<neon::ShaderProgram>(&_render->getApplication(), "neoneuron:activity");

        auto fs = cmrc::resources::get_filesystem();
        shader->addShader(neon::ShaderType::TASK, fs.open("/shader/activity/activity.task"));
        shader->addShader(neon::ShaderType::MESH, fs.open("/shader/activity/activity.mesh"));
        shader->addShader(neon::ShaderType::FRAGMENT, fs.open("/shader/activity/activity.frag"));

        if (auto result = shader->compile(); result.has_value()) {
            neon::error() << result.value();
        } else {
            _shader = std::move(shader);
        }
    }

    std::shared_ptr<neon::Material> ActivityRepresentation::loadMaterial(const Viewport* viewport) const
    {
        auto* app = &_render->getApplication();
        neon::MaterialCreateInfo materialCreateInfo(viewport->getInputFrameBuffer(), _shader);
        materialCreateInfo.rasterizer.cullMode = neon::CullMode::BACK;
        materialCreateInfo.rasterizer.polygonMode = neon::PolygonMode::FILL;
        materialCreateInfo.descriptions.uniformBuffer = viewport->getUniformBuffer();
        materialCreateInfo.descriptions.uniformBindings[UNIFORM_SET] = neon::DescriptorBinding::extra(_uboDescriptor);
        return std::make_shared<neon::Material>(app, "neoneuron:activity", materialCreateInfo);
    }

    void ActivityRepresentation::loadModel()
    {
        auto* app = &_render->getApplication();

        auto drawable = std::make_shared<neon::MeshShaderDrawable>(
            app, "neoneuron:activity", std::unordered_set<std::shared_ptr<neon::Material>>());
        drawable->setGroupsSupplier([](const neon::Model& model) {
            uint32_t amount = model.getInstanceData(0)->getInstanceAmount();
            uint32_t tasks = amount / 1024 + (amount % 1024 != 0);
            return rush::Vec<3, uint32_t>{tasks, 1, 1};
        });

        neon::ModelCreateInfo modelCreateInfo;
        modelCreateInfo.maximumInstances = ACTIVITY_INSTANCES;
        modelCreateInfo.drawables.push_back(drawable);
        modelCreateInfo.uniformBufferBindings[UNIFORM_SET] = neon::ModelBufferBinding::extra(_ubo);

        modelCreateInfo.defineInstanceType<ActivityGPUNeuronData>();
        modelCreateInfo.instanceDataProvider = [this](neon::Application* app, const neon::ModelCreateInfo& info,
                                                      const neon::Model* model) {
            std::vector indices = {
                neon::StorageBufferInstanceData::Slot(sizeof(ActivityGPUNeuronData), sizeof(ActivityGPUNeuronData),
                                                      NEURON_BINDING, _ubo.get()),
                neon::StorageBufferInstanceData::Slot(sizeof(float), sizeof(float), ACTIVITY_BINDING, _ubo.get())};
            return std::vector<neon::InstanceData*>{new neon::StorageBufferInstanceData(app, info, indices)};
        };

        _model = std::make_shared<neon::Model>(app, "neoneuron:activity", modelCreateInfo);
        _render->getRoom()->markUsingModel(_model.get());
    }

    void ActivityRepresentation::onClear()
    {
        _gpuNeurons.clear();
        _sceneBoundingBox = {};
        updateGPURepresentationData();
    }

    void ActivityRepresentation::recalculateBoundingBox()
    {
        if (_gpuNeurons.empty()) {
            _sceneBoundingBox = {};
            return;
        }

        auto min = rush::Vec3f(std::numeric_limits<float>::max());
        auto max = rush::Vec3f(std::numeric_limits<float>::min());

        for (auto& neuron : _gpuNeurons | std::views::values) {
            auto position = neuron.getPosition();
            min = rush::min(min, position);
            max = rush::max(max, position);
        }

        _sceneBoundingBox = rush::AABB<3, float>::fromEdges(min, max);
    }

    void ActivityRepresentation::addNeuronToBoundingBox(const ActivityGPUNeuron& neuron)
    {
        auto min = _sceneBoundingBox.center - _sceneBoundingBox.radius;
        auto max = _sceneBoundingBox.center + _sceneBoundingBox.radius;

        auto position = neuron.getPosition();
        min = rush::min(min, position);
        max = rush::max(max, position);

        _sceneBoundingBox = rush::AABB<3, float>::fromEdges(min, max);
    }

    void ActivityRepresentation::updateGPURepresentationData() const
    {
        ActivityRepresentationData data;
        rush::Vec4f red = {1.0f, 0.0f, 0.0f, 1.0f};
        rush::Vec4f blue = {0.0f, 0.0f, 1.0f, 1.0f};

        for (size_t i = 0; i < ACTIVITY_REPRESENTATION_GRADIENT_SIZE; i++) {
            float n = static_cast<float>(i) / (ACTIVITY_REPRESENTATION_GRADIENT_SIZE - 1);
            data.gradient[i] = rush::mix(red, blue, n);
            data.sizes[i] = rush::mix(2.0f, 1.0f, n);
        }

        data.decay = 1.0f;
        data.activities = _gpuNeurons.size();

        _ubo->uploadData(REPRESENTATION_BINDING, std::move(data));
    }

    ActivityRepresentation::ActivityRepresentation(NeoneuronRender* render) :
        _render(render)
    {
        loadUniformBuffers();
        loadShader();
        loadModel();

        _ubo->uploadData(VOLATILE_BINDING, ActivityRepresentationVolatileData(0.0f));
        _instanceData = _model->getInstanceData(0);
    }

    ActivityRepresentation::~ActivityRepresentation()
    {
        if (_model != nullptr) {
            _render->getRoom()->unmarkUsingModel(_model.get());
        }
    }

    NeoneuronRender* ActivityRepresentation::getRender()
    {
        return _render;
    }

    const NeoneuronRender* ActivityRepresentation::getRender() const
    {
        return _render;
    }

    rush::AABB<3, float> ActivityRepresentation::getSceneBoundingBox() const
    {
        return _sceneBoundingBox;
    }

    void ActivityRepresentation::refreshNeuronProperty(GID neuronId, const std::string& propertyName)
    {
    }

    void ActivityRepresentation::refreshData(const RepositoryView& view)
    {
        auto& newGIDs = view.getNeuronsGIDs();
        std::unordered_set set(newGIDs.begin(), newGIDs.end());

        // Remove
        for (const auto& gid : _neuronsInData) {
            if (!set.contains(gid)) {
                _gpuNeurons.erase(gid);
            }
        }

        // Add
        for (const auto& gid : set) {
            if (!_neuronsInData.contains(gid)) {
                if (auto optional = view.getRepository()->getNeuronAndDataset(gid)) {
                    auto [dataset, neuron] = *optional;
                    auto lock = neuron->readLock();
                    auto prop = dataset->getDataset().getProperties().getPropertyUID(mindset::PROPERTY_TRANSFORM);
                    if (!prop.has_value()) {
                        neon::error() << "Property " << mindset::PROPERTY_TRANSFORM << " not found";
                        continue;
                    }
                    if (auto position = neuron->getProperty<mindset::NeuronTransform>(*prop)) {
                        _gpuNeurons.emplace(std::piecewise_construct, std::forward_as_tuple(gid),
                                            std::forward_as_tuple(_instanceData, gid, position->getPosition(), -10000.0f));
                    } else {
                        neon::error() << "Neuron's position not found";
                    }
                }
            }
        }

        recalculateBoundingBox();
        updateGPURepresentationData();
        _neuronsInData = set;
    }

    void ActivityRepresentation::clearData()
    {
        _gpuNeurons.clear();
        _neuronsInData.clear();
    }

    void ActivityRepresentation::addViewport(const Viewport* viewport)
    {
        if (_viewports.contains(viewport)) {
            return;
        }
        auto fb = viewport->getInputFrameBuffer();
        auto material = loadMaterial(viewport);

        for (auto& mesh : _model->getMeshes()) {
            mesh->getMaterials().insert(material);
        }

        _viewports.emplace(viewport, std::move(material));
    }

    void ActivityRepresentation::removeViewport(const Viewport* viewport)
    {
        auto it = _viewports.find(viewport);
        if (it == _viewports.end()) {
            return;
        }

        for (auto& mesh : _model->getMeshes()) {
            mesh->getMaterials().erase(it->second);
        }

        _viewports.erase(it);
    }

    bool ActivityRepresentation::hasViewport(const Viewport* viewport)
    {
        return _viewports.contains(viewport);
    }

    void ActivityRepresentation::setViewports(const std::unordered_set<const Viewport*>& viewport)
    {
        // First, remove
        for (auto it = _viewports.begin(); it != _viewports.end();) {
            if (!viewport.contains(it->first)) {
                for (auto& mesh : _model->getMeshes()) {
                    mesh->getMaterials().erase(it->second);
                }
                it = _viewports.erase(it);
            } else {
                ++it;
            }
        }

        // Then, add
        for (auto vp : viewport) {
            addViewport(vp);
        }
    }

    size_t ActivityRepresentation::getTotalAllocatedMemory() const
    {
        size_t size = 0;
        for (auto& binding : _uboDescriptor->getBindings()) {
            if (binding.type == neon::UniformBindingType::STORAGE_BUFFER ||
                binding.type == neon::UniformBindingType::UNIFORM_BUFFER) {
                size += binding.size;
            }
        }
        return size;
    }

    size_t ActivityRepresentation::getAllocatedInstanceMemory() const
    {
        size_t size = 0;
        for (auto& data : _model->getInstanceDatas()) {
            size += data->getBytesRequiredPerInstance() * data->getMaximumInstances();
        }
        return size;
    }

    size_t ActivityRepresentation::getUsedInstanceMemory() const
    {
        size_t size = 0;
        for (auto& data : _model->getInstanceDatas()) {
            size += data->getBytesRequiredPerInstance() * data->getInstanceAmount();
        }
        return size;
    }

    float ActivityRepresentation::getUsedInstanceMemoryPercentage() const
    {
        return static_cast<float>(getUsedInstanceMemory()) / static_cast<float>(getAllocatedInstanceMemory());
    }

    void ActivityRepresentation::onTimeChanged(std::chrono::nanoseconds lastTime, std::chrono::nanoseconds newTime,
                                               TimeChangeType type)
    {
        if (!_activity) {
            return;
        }

        if (lastTime > newTime) {
            // Loop detected. Process both segments
            onTimeChanged(lastTime, std::chrono::nanoseconds::max(), type);
            onTimeChanged(std::chrono::nanoseconds(0), newTime, type);
            return;
        }

        for (auto event : _activity->sequence.getRange(lastTime, newTime)) {
            GID gid = {_activity->activityId.datasetId, event.uid};
            auto it = _gpuNeurons.find(gid);
            if (it == _gpuNeurons.end()) {
                continue;
            }

            auto seconds = std::chrono::duration_cast<std::chrono::duration<float>>(event.timepoint);
            it->second.updateActivityValue(seconds.count());
        }

        auto seconds = std::chrono::duration_cast<std::chrono::duration<float>>(newTime);
        _ubo->uploadData(VOLATILE_BINDING, ActivityRepresentationVolatileData(seconds.count()));
    }

    std::vector<ActivityEntry<mindset::TimeGrid<double>>> ActivityRepresentation::getTimeGrids()
    {
        return {};
    }

    std::vector<ActivityEntry<mindset::EventSequence<std::monostate>>> ActivityRepresentation::getEventSequences()
    {
        if (_activity) {
            return {ActivityEntry(_activity->activityId, _activity->name, &_activity->sequence)};
        }

        return {};
    }

    void ActivityRepresentation::setActivity(GID activityId, std::string name,
                                             mindset::EventSequence<std::monostate> sequence)
    {
        _activity = {activityId, std::move(name), std::move(sequence)};
    }

    void ActivityRepresentation::clearActivity()
    {
        _activity = {};
    }

} // namespace neoneuron