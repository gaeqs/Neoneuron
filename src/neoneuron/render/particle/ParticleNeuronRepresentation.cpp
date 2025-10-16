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

#include "ParticleNeuronRepresentation.h"

#include <neoneuron/application/NeoneuronApplication.h>
#include <neoneuron/render/extension/neuron/StaticNeuronColorAndScaleSE.h>

CMRC_DECLARE(resources);

namespace neoneuron
{

    void ParticleNeuronRepresentation::loadUniformBuffers()
    {
        auto* app = &_render->getApplication();

        std::vector bindings = {
            neon::ShaderUniformBinding::uniformBuffer(sizeof(ActivityRepresentationData)),
            neon::ShaderUniformBinding::storageBuffer(sizeof(ActivityGPUNeuronData) * ACTIVITY_INSTANCES),
        };

        _uboDescriptor =
            std::make_shared<neon::ShaderUniformDescriptor>(app, "neoneuron:activity_descriptor", bindings);

        _ubo = std::make_shared<neon::ShaderUniformBuffer>("neoneuron:activity_ubo", _uboDescriptor);
    }

    void ParticleNeuronRepresentation::loadShader()
    {
        std::string include = _colorAndScale->generateShaderCode(COLOR_AND_SCALE_SET);
        neon::IncluderCreateInfo info;
        info.prefetchedIncludes[NeuronColorAndScaleSE::EXTENSION_INCLUDE_NAME] = include;

        auto shader = std::make_shared<neon::ShaderProgram>(&_render->getApplication(), "neoneuron:activity");

        auto fs = cmrc::resources::get_filesystem();
        shader->addShader(neon::ShaderType::TASK, fs.open("/shader/activity/activity.task"));
        shader->addShader(neon::ShaderType::MESH, fs.open("/shader/activity/activity.mesh"));
        shader->addShader(neon::ShaderType::FRAGMENT, fs.open("/shader/activity/activity.frag"));

        if (auto result = shader->compile(info); result.has_value()) {
            neon::error() << result.value();
        } else {
            _shader = std::move(shader);
        }
    }

    std::shared_ptr<neon::Material> ParticleNeuronRepresentation::loadMaterial(const Viewport* viewport) const
    {
        auto* app = &_render->getApplication();
        neon::MaterialCreateInfo materialCreateInfo(viewport->getInputFrameBuffer(), _shader);
        materialCreateInfo.rasterizer.cullMode = neon::CullMode::BACK;
        materialCreateInfo.rasterizer.polygonMode = neon::PolygonMode::FILL;
        materialCreateInfo.descriptions.uniformBuffer = viewport->getUniformBuffer();
        materialCreateInfo.descriptions.uniformBindings[UNIFORM_SET] = neon::DescriptorBinding::extra(_uboDescriptor);

        if (auto extensionUbo = _colorAndScale->getUBODescriptor(); extensionUbo) {
            materialCreateInfo.descriptions.uniformBindings[COLOR_AND_SCALE_SET] =
                neon::DescriptorBinding::extra(*extensionUbo);
        }

        return std::make_shared<neon::Material>(app, "neoneuron:activity", materialCreateInfo);
    }

    void ParticleNeuronRepresentation::loadModel()
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

        if (auto extensionUbo = _colorAndScale->getUBO(); extensionUbo) {
            modelCreateInfo.uniformBufferBindings[COLOR_AND_SCALE_SET] = neon::ModelBufferBinding::extra(*extensionUbo);
        }

        modelCreateInfo.defineInstanceType<ActivityGPUNeuronData, float>();
        modelCreateInfo.instanceDataProvider = [this](neon::Application* app, const neon::ModelCreateInfo& info,
                                                      const neon::Model* model) {
            std::vector indices = {neon::StorageBufferInstanceData::Slot(
                sizeof(ActivityGPUNeuronData), sizeof(ActivityGPUNeuronData), NEURON_BINDING, _ubo.get())};
            return std::vector<neon::InstanceData*>{new neon::StorageBufferInstanceData(app, info, indices)};
        };

        _model = std::make_shared<neon::Model>(app, "neoneuron:activity", modelCreateInfo);
        _render->getRoom()->markUsingModel(_model.get());
    }

    void ParticleNeuronRepresentation::onClear()
    {
        _gpuNeurons.clear();
        _sceneBoundingBox = {};
        updateGPURepresentationData();
    }

    void ParticleNeuronRepresentation::recalculateBoundingBox()
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

    void ParticleNeuronRepresentation::addNeuronToBoundingBox(const ParticleGPUNeuron& neuron)
    {
        auto min = _sceneBoundingBox.center - _sceneBoundingBox.radius;
        auto max = _sceneBoundingBox.center + _sceneBoundingBox.radius;

        auto position = neuron.getPosition();
        min = rush::min(min, position);
        max = rush::max(max, position);

        _sceneBoundingBox = rush::AABB<3, float>::fromEdges(min, max);
    }

    void ParticleNeuronRepresentation::updateGPURepresentationData() const
    {
        ActivityRepresentationData data;
        data.activities = _gpuNeurons.size();
        _ubo->uploadData(REPRESENTATION_BINDING, std::move(data));
    }

    ParticleNeuronRepresentation::ParticleNeuronRepresentation(NeoneuronRender* render) :
        _render(render),
        _colorAndScale(std::make_shared<StaticNeuronColorAndScaleSE>(&render->getApplication()))
    {
        loadUniformBuffers();
        loadShader();
        loadModel();

        _instanceData = _model->getInstanceData(0);
    }

    ParticleNeuronRepresentation::~ParticleNeuronRepresentation()
    {
        for (const auto& gid : _gpuNeurons | std::views::keys) {
            _colorAndScale->unregisterElement(gid);
        }

        if (_model != nullptr) {
            _render->getRoom()->unmarkUsingModel(_model.get());
        }
    }

    NeoneuronRender* ParticleNeuronRepresentation::getRender()
    {
        return _render;
    }

    const NeoneuronRender* ParticleNeuronRepresentation::getRender() const
    {
        return _render;
    }

    rush::AABB<3, float> ParticleNeuronRepresentation::getSceneBoundingBox() const
    {
        return _sceneBoundingBox;
    }

    void ParticleNeuronRepresentation::refreshNeuronProperty(GID neuronId, const std::string& propertyName)
    {
        if (propertyName == mindset::PROPERTY_TRANSFORM) {
            if (auto it = _gpuNeurons.find(neuronId); it != _gpuNeurons.end()) {
                if (auto optional =
                        _render->getNeoneuronApplication()->getRepository().getNeuronAndDataset(it->second.getGID())) {
                    auto [dataset, neuron] = *optional;
                    auto lock = neuron->readLock();
                    auto prop = dataset->getDataset().getProperties().getPropertyUID(mindset::PROPERTY_TRANSFORM);
                    if (!prop.has_value()) {
                        neon::error() << "Property " << mindset::PROPERTY_TRANSFORM << " not found";
                        return;
                    }
                    if (auto position = neuron->getProperty<mindset::NeuronTransform>(*prop)) {
                        it->second.setPosition(position->getPosition());
                        recalculateBoundingBox();
                    }
                }
            }
        }
    }

    void ParticleNeuronRepresentation::refreshData(const RepositoryView& view)
    {
        auto& newGIDs = view.getNeuronsGIDs();
        std::unordered_set set(newGIDs.begin(), newGIDs.end());

        // Remove
        for (const auto& gid : _neuronsInData) {
            if (!set.contains(gid)) {
                _gpuNeurons.erase(gid);
                _colorAndScale->unregisterElement(gid);
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
                        auto [it, ok] =
                            _gpuNeurons.emplace(std::piecewise_construct, std::forward_as_tuple(gid),
                                                std::forward_as_tuple(_instanceData, gid, position->getPosition()));
                        if (ok) {
                            it->second.setColorAndScaleIndex(_colorAndScale->registerElement(gid));
                        }
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

    void ParticleNeuronRepresentation::clearData()
    {
        _gpuNeurons.clear();
        _neuronsInData.clear();
    }

    void ParticleNeuronRepresentation::addViewport(const Viewport* viewport)
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

    void ParticleNeuronRepresentation::removeViewport(const Viewport* viewport)
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

    bool ParticleNeuronRepresentation::hasViewport(const Viewport* viewport)
    {
        return _viewports.contains(viewport);
    }

    void ParticleNeuronRepresentation::setViewports(const std::unordered_set<const Viewport*>& viewport)
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

    size_t ParticleNeuronRepresentation::getTotalAllocatedMemory() const
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

    size_t ParticleNeuronRepresentation::getAllocatedInstanceMemory() const
    {
        size_t size = 0;
        for (auto& data : _model->getInstanceDatas()) {
            size += data->getBytesRequiredPerInstance() * data->getMaximumInstances();
        }
        return size;
    }

    size_t ParticleNeuronRepresentation::getUsedInstanceMemory() const
    {
        size_t size = 0;
        for (auto& data : _model->getInstanceDatas()) {
            size += data->getBytesRequiredPerInstance() * data->getInstanceAmount();
        }
        return size;
    }

    float ParticleNeuronRepresentation::getUsedInstanceMemoryPercentage() const
    {
        return static_cast<float>(getUsedInstanceMemory()) / static_cast<float>(getAllocatedInstanceMemory());
    }

    void ParticleNeuronRepresentation::setColorAndScale(std::shared_ptr<NeuronColorAndScaleSE> colorAndScale)
    {
        for (const auto& gid : _gpuNeurons | std::views::keys) {
            _colorAndScale->unregisterElement(gid);
        }

        if (colorAndScale == nullptr) {
            // Default
            _colorAndScale = std::make_shared<StaticNeuronColorAndScaleSE>(&_render->getApplication());
        } else {
            _colorAndScale = std::move(colorAndScale);
        }

        loadShader();

        for (auto& [vp, material] : _viewports) {
            material = loadMaterial(vp);
        }

        for (auto& mesh : _model->getMeshes()) {
            auto& set = mesh->getMaterials();
            set.clear();
            set.reserve(_viewports.size());
            for (auto& materials : _viewports | std::views::values) {
                set.insert(materials);
            }
        }

        if (auto ubo = _colorAndScale->getUBO(); ubo) {
            auto binding = neon::ModelBufferBinding::extra(*ubo);
            _model->getUniformBufferBindings()[COLOR_AND_SCALE_SET] = binding;
        } else {
            _model->getUniformBufferBindings().erase(COLOR_AND_SCALE_SET);
        }

        for (auto& gpu : _gpuNeurons | std::views::values) {
            gpu.setColorAndScaleIndex(_colorAndScale->registerElement(gpu.getGID()));
        }
    }

} // namespace neoneuron