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

#include "SynapseRepresentation.h"

#include "GPUSynapse.h"

#include <neoneuron/application/NeoneuronApplication.h>
#include <neoneuron/render/NeoneuronRender.h>

CMRC_DECLARE(resources);

namespace neoneuron
{

    void SynapseRepresentation::loadUniformBuffers()
    {
        auto* app = &_render->getApplication();

        std::vector bindings = {
            neon::ShaderUniformBinding::uniformBuffer(sizeof(SynapseRepresentationData)),
            neon::ShaderUniformBinding::storageBuffer(sizeof(GPUSynapseData) * SYNAPSE_INSTANCES),
        };

        _uboDescriptor = std::make_shared<neon::ShaderUniformDescriptor>(app, "neoneuron:synapse_descriptor", bindings);

        _ubo = std::make_shared<neon::ShaderUniformBuffer>("neoneuron:synapse_ubo", _uboDescriptor);
    }

    void SynapseRepresentation::loadShader()
    {
        auto shader = std::make_shared<neon::ShaderProgram>(&_render->getApplication(), "neoneuron:synapse");

        auto fs = cmrc::resources::get_filesystem();
        shader->addShader(neon::ShaderType::TASK, fs.open("/shader/synapse/synapse.task"));
        shader->addShader(neon::ShaderType::MESH, fs.open("/shader/synapse/synapse.mesh"));
        shader->addShader(neon::ShaderType::FRAGMENT, fs.open("/shader/synapse/synapse.frag"));

        if (auto result = shader->compile(); result.has_value()) {
            neon::error() << result.value();
        } else {
            _shader = std::move(shader);
        }
    }

    std::shared_ptr<neon::Material> SynapseRepresentation::loadMaterial(const Viewport* viewport) const
    {
        auto* app = &_render->getApplication();
        neon::MaterialCreateInfo materialCreateInfo(viewport->getInputFrameBuffer(), _shader);
        materialCreateInfo.rasterizer.cullMode = neon::CullMode::BACK;
        materialCreateInfo.rasterizer.polygonMode = neon::PolygonMode::FILL;
        materialCreateInfo.descriptions.uniformBuffer = viewport->getUniformBuffer();
        materialCreateInfo.descriptions.uniformBindings[UNIFORM_SET] = neon::DescriptorBinding::extra(_uboDescriptor);
        return std::make_shared<neon::Material>(app, "neoneuron:synapse", materialCreateInfo);
    }

    void SynapseRepresentation::loadModel()
    {
        auto* app = &_render->getApplication();

        auto drawable = std::make_shared<neon::MeshShaderDrawable>(
            app, "neoneuron:synapse", std::unordered_set<std::shared_ptr<neon::Material>>());
        drawable->setGroupsSupplier([](const neon::Model& model) {
            uint32_t amount = model.getInstanceData(0)->getInstanceAmount();
            uint32_t tasks = amount / 1024 + (amount % 1024 != 0);
            return rush::Vec<3, uint32_t>{tasks, 1, 1};
        });

        neon::ModelCreateInfo modelCreateInfo;
        modelCreateInfo.maximumInstances = SYNAPSE_INSTANCES;
        modelCreateInfo.drawables.push_back(drawable);
        modelCreateInfo.uniformBufferBindings[UNIFORM_SET] = neon::ModelBufferBinding::extra(_ubo);

        modelCreateInfo.defineInstanceType<GPUSynapseData>();
        modelCreateInfo.instanceDataProvider = [this](neon::Application* app, const neon::ModelCreateInfo& info,
                                                      const neon::Model* model) {
            std::vector indices = {neon::StorageBufferInstanceData::Slot(sizeof(GPUSynapseData), sizeof(GPUSynapseData),
                                                                         SYNAPSE_BINDING, _ubo.get())};
            return std::vector<neon::InstanceData*>{new neon::StorageBufferInstanceData(app, info, indices)};
        };

        _model = std::make_shared<neon::Model>(app, "neoneuron:synapse", modelCreateInfo);
        _render->getRoom()->markUsingModel(_model.get());
    }

    void SynapseRepresentation::onClear()
    {
        _gpuSynapses.clear();
        _sceneBoundingBox = {};
        updateGPURepresentationData();
    }

    void SynapseRepresentation::recalculateBoundingBox()
    {
        if (_gpuSynapses.empty()) {
            _sceneBoundingBox = {};
            return;
        }

        auto min = rush::Vec3f(std::numeric_limits<float>::max());
        auto max = rush::Vec3f(std::numeric_limits<float>::min());

        for (auto& synapse : _gpuSynapses | std::views::values) {
            auto pre = synapse.getPrePosition();
            auto post = synapse.getPostPosition();
            if (pre) {
                min = rush::min(min, *pre);
                max = rush::max(max, *pre);
            }
            if (post) {
                min = rush::min(min, *post);
                max = rush::max(max, *post);
            }
        }

        _sceneBoundingBox = rush::AABB<3, float>::fromEdges(min, max);
    }

    void SynapseRepresentation::addSynapseToBoundingBox(const GPUSynapse& synapse)
    {
        auto min = _sceneBoundingBox.center - _sceneBoundingBox.radius;
        auto max = _sceneBoundingBox.center + _sceneBoundingBox.radius;

        auto pre = synapse.getPrePosition();
        auto post = synapse.getPostPosition();

        if (pre) {
            min = rush::min(min, *pre);
            max = rush::max(max, *pre);
        }
        if (post) {
            min = rush::min(min, *post);
            max = rush::max(max, *post);
        }

        _sceneBoundingBox = rush::AABB<3, float>::fromEdges(min, max);
    }

    void SynapseRepresentation::updateGPURepresentationData() const
    {
        _ubo->uploadData(REPRESENTATION_BINDING, SynapseRepresentationData(_gpuSynapses.size()));
    }

    SynapseRepresentation::SynapseRepresentation(NeoneuronRender* render) :
        _render(render),
        _createInfo()
    {
        loadUniformBuffers();
        loadShader();
        loadModel();

        _createInfo =
            GPUSynapseCreateInfo(&render->getNeoneuronApplication()->getRepository(), _model->getInstanceData(0));
    }

    SynapseRepresentation::~SynapseRepresentation()
    {
        if (_model != nullptr) {
            _render->getRoom()->unmarkUsingModel(_model.get());
        }
    }

    NeoneuronRender* SynapseRepresentation::getRender()
    {
        return _render;
    }

    const NeoneuronRender* SynapseRepresentation::getRender() const
    {
        return _render;
    }

    rush::AABB<3, float> SynapseRepresentation::getSceneBoundingBox() const
    {
        return _sceneBoundingBox;
    }

    void SynapseRepresentation::refreshNeuronProperty(GID neuronId, const std::string& propertyName)
    {
    }

    void SynapseRepresentation::refreshData(const RepositoryView& view)
    {
        auto& newGIDs = view.getSynapses();
        std::unordered_set set(newGIDs.begin(), newGIDs.end());

        // Remove
        for (const auto& gid : _synapsesInDataset) {
            if (!set.contains(gid)) {
                _gpuSynapses.erase(gid);
            }
        }

        // Add
        for (const auto& gid : set) {
            if (!_synapsesInDataset.contains(gid)) {
                if (auto optional = view.getRepository()->getSynapseAndDataset(gid)) {
                    auto [dataset, synapse] = *optional;
                    _gpuSynapses.emplace(std::piecewise_construct, std::forward_as_tuple(gid),
                                         std::forward_as_tuple(_createInfo, gid, synapse));
                }
            }
        }

        recalculateBoundingBox();
        updateGPURepresentationData();
        _synapsesInDataset = set;
    }

    void SynapseRepresentation::clearData()
    {
        _gpuSynapses.clear();
        _synapsesInDataset.clear();
    }

    void SynapseRepresentation::addViewport(Viewport* viewport)
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

    void SynapseRepresentation::removeViewport(Viewport* viewport)
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

    void SynapseRepresentation::setViewports(const std::unordered_set<Viewport*>& viewport)
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
} // namespace neoneuron
