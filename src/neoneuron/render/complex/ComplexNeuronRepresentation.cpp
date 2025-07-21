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

#include "ComplexNeuronRepresentation.h"

#include "neoneuron/render/Viewport.h"

#include <neon/util/Chronometer.h>
#include <neon/util/task/Coroutine.h>
#include <neoneuron/render/NeoneuronRender.h>
#include <neoneuron/application/NeoneuronApplication.h>

CMRC_DECLARE(resources);

namespace
{
    neon::Coroutine<> deleteCoroutine(std::vector<std::shared_ptr<neon::Material>> materials)
    {
        for (size_t i = 0; i < 10; ++i) {
            co_yield neon::WaitForNextFrame();
        }
        materials.clear();
    }

    neoneuron::ComplexNeuron process(neoneuron::ComplexNeuronProcessData&& data)
    {
        return neoneuron::ComplexNeuron(data.gid, data.dataset, data.neuron);
    }
} // namespace

namespace neoneuron
{
    void ComplexNeuronRepresentation::loadUniformBuffers()
    {
        auto* app = &_render->getApplication();

        std::vector bindings = {
            // GLOBAL
            neon::ShaderUniformBinding::uniformBuffer(sizeof(ComplexNeuronRepresentationData)),
            // NEURONS
            neon::ShaderUniformBinding::storageBuffer(sizeof(ComplexGPUNeuronGlobalData) * SOMA_INSTANCES),
            // SEGMENTS
            neon::ShaderUniformBinding::storageBuffer(sizeof(ComplexGPUNeuronSegment) * SEGMENT_INSTANCES),
            // JOINTS
            neon::ShaderUniformBinding::storageBuffer(sizeof(ComplexGPUNeuronJoint) * JOINT_INSTANCES),
            // SOMAS
            neon::ShaderUniformBinding::storageBuffer(sizeof(ComplexGPUNeuronSoma) * SOMA_INSTANCES),
            // SELECTION
            neon::ShaderUniformBinding::storageBuffer(sizeof(ComplexGPUSelectionData) * SEGMENT_INSTANCES),
            // SOMA GPU DATA
            neon::ShaderUniformBinding::storageBuffer(STORAGE_PER_SOMA * SOMA_INSTANCES),
            // SECTION GPU DATA
            neon::ShaderUniformBinding::storageBuffer(STORAGE_PER_SECTION * SEGMENT_INSTANCES),

            neon::ShaderUniformBinding::storageBuffer(MAX_SAVE_VERTICES * sizeof(rush::Vec4f),
                                                      neon::UniformBindingBufferType::SIMPLE),

            neon::ShaderUniformBinding::storageBuffer(MAX_SAVE_VERTICES * sizeof(rush::Vec4i),
                                                      neon::UniformBindingBufferType::SIMPLE),
        };

        _uboDescriptor =
            std::make_shared<neon::ShaderUniformDescriptor>(app, "neoneuron:complex_neuron_descriptor", bindings);

        _ubo = std::make_shared<neon::ShaderUniformBuffer>("neoneuron:complex_neuron_ubo", _uboDescriptor);

        std::vector slots = {
            neon::StorageBufferInstanceData::Slot(sizeof(ComplexGPUNeuronGlobalData),
                                                  sizeof(ComplexGPUNeuronGlobalData), NEURON_BINDING, _ubo.get()),
        };

        std::vector<std::type_index> types = {typeid(ComplexGPUNeuronGlobalData)};

        _globalInstanceData =
            std::make_shared<neon::StorageBufferInstanceData>(app, static_cast<uint32_t>(SOMA_INSTANCES), types, slots);
    }

    void ComplexNeuronRepresentation::loadNeuronShader()
    {
        _neuronShader = std::make_shared<neon::ShaderProgram>(&_render->getApplication(), "Neuron");

        auto fs = cmrc::resources::get_filesystem();
        _neuronShader->addShader(neon::ShaderType::TASK, fs.open("/shader/neuron/complex/neuron.task"));
        _neuronShader->addShader(neon::ShaderType::MESH, fs.open("/shader/neuron/complex/neuron.mesh"));
        _neuronShader->addShader(neon::ShaderType::FRAGMENT, fs.open("/shader/neuron/complex/neuron.frag"));

        if (auto result = _neuronShader->compile(); result.has_value()) {
            neon::error() << result.value();
        }
    }

    void ComplexNeuronRepresentation::loadJointShader()
    {
        _jointShader = std::make_shared<neon::ShaderProgram>(&_render->getApplication(), "Neuron");

        auto fs = cmrc::resources::get_filesystem();
        _jointShader->addShader(neon::ShaderType::TASK, fs.open("/shader/neuron/complex/joint.task"));
        _jointShader->addShader(neon::ShaderType::MESH, fs.open("/shader/neuron/complex/joint.mesh"));
        _jointShader->addShader(neon::ShaderType::FRAGMENT, fs.open("/shader/neuron/complex/joint.frag"));

        if (auto result = _jointShader->compile(); result.has_value()) {
            neon::error() << result.value();
        }
    }

    void ComplexNeuronRepresentation::loadSomaShader()
    {
        auto shader = std::make_shared<neon::ShaderProgram>(&_render->getApplication(), "Neuron");

        auto fs = cmrc::resources::get_filesystem();
        shader->addShader(neon::ShaderType::TASK, fs.open("/shader/neuron/complex/soma.task"));
        shader->addShader(neon::ShaderType::MESH, fs.open("/shader/neuron/complex/soma.mesh"));
        shader->addShader(neon::ShaderType::FRAGMENT, fs.open("/shader/neuron/complex/soma.frag"));

        if (auto result = shader->compile(); result.has_value()) {
            neon::error() << result.value();
        } else {
            _somaShader = std::move(shader);
        }
    }

    std::shared_ptr<neon::Material> ComplexNeuronRepresentation::loadSegmentMaterial(const Viewport* viewport) const
    {
        auto* app = &_render->getApplication();
        neon::MaterialCreateInfo materialCreateInfo(viewport->getInputFrameBuffer(), _neuronShader);
        materialCreateInfo.rasterizer.cullMode = neon::CullMode::BACK;
        materialCreateInfo.rasterizer.polygonMode = _wireframe ? neon::PolygonMode::LINE : neon::PolygonMode::FILL;
        materialCreateInfo.descriptions.uniformBuffer = viewport->getUniformBuffer();
        materialCreateInfo.descriptions.uniformBindings[UNIFORM_SET] = neon::DescriptorBinding::extra(_uboDescriptor);
        return std::make_shared<neon::Material>(app, "neoneuron:segment", materialCreateInfo);
    }

    std::shared_ptr<neon::Material> ComplexNeuronRepresentation::loadJointMaterial(const Viewport* viewport) const
    {
        auto* app = &_render->getApplication();
        neon::MaterialCreateInfo materialCreateInfo(viewport->getInputFrameBuffer(), _jointShader);
        materialCreateInfo.rasterizer.cullMode = neon::CullMode::BACK;
        materialCreateInfo.rasterizer.polygonMode = _wireframe ? neon::PolygonMode::LINE : neon::PolygonMode::FILL;
        materialCreateInfo.descriptions.uniformBuffer = viewport->getUniformBuffer();
        materialCreateInfo.descriptions.uniformBindings[UNIFORM_SET] = neon::DescriptorBinding::extra(_uboDescriptor);
        return std::make_shared<neon::Material>(app, "neoneuron:joint", materialCreateInfo);
    }

    std::shared_ptr<neon::Material> ComplexNeuronRepresentation::loadSomaMaterial(const Viewport* viewport) const
    {
        auto* app = &_render->getApplication();
        neon::MaterialCreateInfo materialCreateInfo(viewport->getInputFrameBuffer(), _somaShader);
        materialCreateInfo.rasterizer.cullMode = neon::CullMode::BACK;
        materialCreateInfo.rasterizer.polygonMode = _wireframe ? neon::PolygonMode::LINE : neon::PolygonMode::FILL;
        materialCreateInfo.descriptions.uniformBuffer = viewport->getUniformBuffer();
        materialCreateInfo.descriptions.uniformBindings[UNIFORM_SET] = neon::DescriptorBinding::extra(_uboDescriptor);
        return std::make_shared<neon::Material>(app, "neoneuron:soma", materialCreateInfo);
    }

    void ComplexNeuronRepresentation::loadNeuronModel()
    {
        auto* app = &_render->getApplication();

        auto drawable = std::make_shared<neon::MeshShaderDrawable>(
            app, "neoneuron:complex_segment", std::unordered_set<std::shared_ptr<neon::Material>>());
        drawable->setGroupsSupplier([](const neon::Model& model) {
            uint32_t amount = model.getInstanceData(0)->getInstanceAmount();
            uint32_t tasks = amount / 32 + (amount % 32 != 0);
            return rush::Vec<3, uint32_t>{tasks, 1, 1};
        });

        neon::ModelCreateInfo modelCreateInfo;
        modelCreateInfo.maximumInstances = SEGMENT_INSTANCES;
        modelCreateInfo.drawables.push_back(drawable);
        modelCreateInfo.uniformBufferBindings[UNIFORM_SET] = neon::ModelBufferBinding::extra(_ubo);

        modelCreateInfo.defineInstanceType<ComplexGPUNeuronSegment>();
        modelCreateInfo.instanceDataProvider = [this](neon::Application* app, const neon::ModelCreateInfo& info,
                                                      const neon::Model* model) {
            std::vector indices = {neon::StorageBufferInstanceData::Slot(
                sizeof(ComplexGPUNeuronSegment), sizeof(ComplexGPUNeuronSegment), SEGMENT_BINDING, _ubo.get())};
            return std::vector<neon::InstanceData*>{new neon::StorageBufferInstanceData(app, info, indices)};
        };

        _segmentModel = std::make_shared<neon::Model>(app, "Neuron", modelCreateInfo);
        if (_drawSegments) {
            _render->getRoom()->markUsingModel(_segmentModel.get());
        }
    }

    void ComplexNeuronRepresentation::loadJointModel()
    {
        auto* app = &_render->getApplication();

        auto drawable = std::make_shared<neon::MeshShaderDrawable>(
            app, "neoneuron:complex_joint", std::unordered_set<std::shared_ptr<neon::Material>>());
        drawable->setGroupsSupplier([](const neon::Model& model) {
            uint32_t amount = model.getInstanceData(0)->getInstanceAmount();
            uint32_t tasks = amount / 32 + (amount % 32 != 0);
            return rush::Vec<3, uint32_t>{tasks, 1, 1};
        });

        neon::ModelCreateInfo modelCreateInfo;
        modelCreateInfo.maximumInstances = JOINT_INSTANCES;
        modelCreateInfo.drawables.push_back(drawable);
        modelCreateInfo.uniformBufferBindings[UNIFORM_SET] = neon::ModelBufferBinding::extra(_ubo);

        modelCreateInfo.defineInstanceType<ComplexGPUNeuronSegment>();
        modelCreateInfo.instanceDataProvider = [this](neon::Application* app, const neon::ModelCreateInfo& info,
                                                      const neon::Model* model) {
            std::vector indices = {
                neon::StorageBufferInstanceData::Slot(sizeof(ComplexGPUNeuronJoint), sizeof(ComplexGPUNeuronJoint),
                                                      JOINT_BINDING, _ubo.get()),
            };
            return std::vector<neon::InstanceData*>{new neon::StorageBufferInstanceData(app, info, indices)};
        };

        _jointModel = std::make_shared<neon::Model>(app, "Joint", modelCreateInfo);
        if (_drawJoints) {
            _render->getRoom()->markUsingModel(_jointModel.get());
        }
    }

    void ComplexNeuronRepresentation::loadSomaModel()
    {
        auto* app = &_render->getApplication();

        auto drawable = std::make_shared<neon::MeshShaderDrawable>(
            app, "neoneuron:complex_soma", std::unordered_set<std::shared_ptr<neon::Material>>());
        drawable->setGroupsSupplier([](const neon::Model& model) {
            return rush::Vec<3, uint32_t>{model.getInstanceData(0)->getInstanceAmount(), 1, 1};
        });

        neon::ModelCreateInfo modelCreateInfo;
        modelCreateInfo.maximumInstances = SOMA_INSTANCES;
        modelCreateInfo.drawables.push_back(drawable);
        modelCreateInfo.uniformBufferBindings[UNIFORM_SET] = neon::ModelBufferBinding::extra(_ubo);

        modelCreateInfo.defineInstanceType<ComplexGPUNeuronSegment>();
        modelCreateInfo.instanceDataProvider = [this](neon::Application* app, const neon::ModelCreateInfo& info,
                                                      const neon::Model* model) {
            std::vector indices = {
                neon::StorageBufferInstanceData::Slot(sizeof(ComplexGPUNeuronSoma), sizeof(ComplexGPUNeuronSoma),
                                                      SOMA_BINDING, _ubo.get()),
            };
            return std::vector<neon::InstanceData*>{new neon::StorageBufferInstanceData(app, info, indices)};
        };

        _somaModel = std::make_shared<neon::Model>(app, "Soma", modelCreateInfo);
        if (_drawSomas) {
            _render->getRoom()->markUsingModel(_somaModel.get());
        }
    }

    void ComplexNeuronRepresentation::combineBoundingBoxes(const rush::AABB<3, float>& aabb)
    {
        auto min = rush::min(aabb.center - aabb.radius, _sceneBoundingBox.center - _sceneBoundingBox.radius);
        auto max = rush::max(aabb.center + aabb.radius, _sceneBoundingBox.center + _sceneBoundingBox.radius);
        _sceneBoundingBox = rush::AABB<3, float>::fromEdges(min, max);
    }

    void ComplexNeuronRepresentation::recalculateBoundingBox()
    {
        if (_neurons.empty()) {
            _sceneBoundingBox = {};
            return;
        }

        auto it = _neurons.begin();
        auto end = _neurons.end();

        auto bb = it->second.neuron.getBoundingBox();
        auto min = bb.center - bb.radius;
        auto max = bb.center + bb.radius;

        for (; it != end; ++it) {
            bb = it->second.neuron.getBoundingBox();
            min = rush::min(min, bb.center - bb.radius);
            max = rush::max(max, bb.center + bb.radius);
        }

        _sceneBoundingBox = rush::AABB<3, float>::fromEdges(min, max);
    }

    void ComplexNeuronRepresentation::recreateMaterials()
    {
        for (auto& [vp, materials] : _viewports) {
            materials.segment = loadSegmentMaterial(vp);
            materials.joint = loadJointMaterial(vp);
            materials.soma = loadSomaMaterial(vp);
        }

        for (auto& mesh : _segmentModel->getMeshes()) {
            auto& set = mesh->getMaterials();
            set.clear();
            set.reserve(_viewports.size());
            for (auto& materials : _viewports | std::views::values) {
                set.insert(materials.segment);
            }
        }

        for (auto& mesh : _jointModel->getMeshes()) {
            auto& set = mesh->getMaterials();
            set.clear();
            set.reserve(_viewports.size());
            for (auto& materials : _viewports | std::views::values) {
                set.insert(materials.joint);
            }
        }

        for (auto& mesh : _somaModel->getMeshes()) {
            auto& set = mesh->getMaterials();
            set.clear();
            set.reserve(_viewports.size());
            for (auto& materials : _viewports | std::views::values) {
                set.insert(materials.soma);
            }
        }
    }

    void ComplexNeuronRepresentation::postProcess()
    {
        _neuronProcessor.fetchResults([this](ComplexNeuron&& neuron) { addComplexNeuron(std::move(neuron)); });
    }

    void ComplexNeuronRepresentation::addComplexNeuron(ComplexNeuron&& complex)
    {
        auto bb = complex.getBoundingBox();
        auto currentFrame = _render->getApplication().getCurrentFrameInformation().currentFrame;

        ComplexGPUNeuron gpu(_globalInstanceData, _segmentModel, _jointModel, _somaModel, 0, 0, 0, &complex,
                             currentFrame);

        _neurons.emplace(std::piecewise_construct, std::forward_as_tuple(complex.getGID()),
                         std::forward_as_tuple(std::move(complex), std::move(gpu)));
        if (_neurons.size() == 1) {
            _sceneBoundingBox = bb;
        } else {
            combineBoundingBoxes(bb);
        }

        refreshSelectionData();
        updateGPURepresentationData();
    }

    void ComplexNeuronRepresentation::onSelectionEvent(SelectionEvent event)
    {
        if (!event.segmentsChanged) {
            return;
        }

        refreshSelectionData();
    }

    void ComplexNeuronRepresentation::refreshSelectionData()
    {
        auto data = static_cast<ComplexGPUSelectionData*>(_ubo->fetchData(SELECTION_BINDING));

        for (uint32_t id : _selection) {
            (data + id)->selected = false;
        }

        _selection.clear();

        for (auto& [neuronId, segmentId] : _render->getNeoneuronApplication()->getSelector().getSelectedSegments()) {
            auto gpuNeuron = findGPUNeuron(neuronId);
            if (!gpuNeuron) {
                continue;
            }

            auto segment = gpuNeuron.value()->findSegment(segmentId);
            if (!segment.has_value()) {
                continue;
            }

            _selection.push_back(*segment.value().id);
            (data + *segment.value().id)->selected = true;
        }
    }

    void ComplexNeuronRepresentation::updateGPURepresentationData() const
    {
        _ubo->uploadData(REPRESENTATION_BINDING,
                         ComplexNeuronRepresentationData(getSectionsAmount(), getJointsAmount()));
    }

    void ComplexNeuronRepresentation::removeMaterials(const Materials& materials)
    {
        for (auto& mesh : _segmentModel->getMeshes()) {
            mesh->getMaterials().erase(materials.segment);
        }
        for (auto& mesh : _jointModel->getMeshes()) {
            mesh->getMaterials().erase(materials.joint);
        }
        for (auto& mesh : _somaModel->getMeshes()) {
            mesh->getMaterials().erase(materials.soma);
        }
    }

    ComplexNeuronRepresentation::ComplexNeuronRepresentation(NeoneuronRender* render) :
        _render(render),
        _neuronProcessor(&render->getApplication().getTaskRunner(), process),
        _wireframe(false),
        _drawSegments(true),
        _drawJoints(true),
        _drawSomas(true)
    {
        loadUniformBuffers();
        loadNeuronShader();
        loadJointShader();
        loadSomaShader();
        loadNeuronModel();
        loadJointModel();
        loadSomaModel();

        _selectionListener = [this](SelectionEvent event) { onSelectionEvent(event); };
        _processorListener = [this](GID) { postProcess(); };

        render->getNeoneuronApplication()->getSelector().onSelectionEvent() += _selectionListener;
        _neuronProcessor.onProcessedEvent() += _processorListener;
    }

    ComplexNeuronRepresentation::~ComplexNeuronRepresentation()
    {
        if (_segmentModel != nullptr) {
            _render->getRoom()->unmarkUsingModel(_segmentModel.get());
        }
        if (_jointModel != nullptr) {
            _render->getRoom()->unmarkUsingModel(_jointModel.get());
        }
        if (_somaModel != nullptr) {
            _render->getRoom()->unmarkUsingModel(_somaModel.get());
        }

        // std::vector materials = {_neuronMaterial, _jointMaterial, _somaMaterial};
        //_render->getApplication().getTaskRunner().launchCoroutine(deleteCoroutine(std::move(materials)));
    }

    NeoneuronRender* ComplexNeuronRepresentation::getRender()
    {
        return _render;
    }

    const NeoneuronRender* ComplexNeuronRepresentation::getRender() const
    {
        return _render;
    }

    size_t ComplexNeuronRepresentation::getNeuronsAmount() const
    {
        return _neurons.size();
    }

    size_t ComplexNeuronRepresentation::getSectionsAmount() const
    {
        return _segmentModel->getInstanceData(0)->getInstanceAmount();
    }

    size_t ComplexNeuronRepresentation::getJointsAmount() const
    {
        return _jointModel->getInstanceData(0)->getInstanceAmount();
    }

    size_t ComplexNeuronRepresentation::getSomasAmount() const
    {
        return _somaModel->getInstanceData(0)->getInstanceAmount();
    }

    std::optional<ComplexNeuron*> ComplexNeuronRepresentation::findNeuron(GID gid)
    {
        auto it = _neurons.find(gid);
        if (it == _neurons.end()) {
            return {};
        }

        return &it->second.neuron;
    }

    std::optional<const ComplexNeuron*> ComplexNeuronRepresentation::findNeuron(GID gid) const
    {
        auto it = _neurons.find(gid);
        if (it == _neurons.end()) {
            return {};
        }

        return &it->second.neuron;
    }

    std::optional<ComplexGPUNeuron*> ComplexNeuronRepresentation::findGPUNeuron(GID gid)
    {
        auto it = _neurons.find(gid);
        if (it == _neurons.end()) {
            return {};
        }

        return &it->second.gpu;
    }

    std::optional<const ComplexGPUNeuron*> ComplexNeuronRepresentation::findGPUNeuron(GID gid) const
    {
        auto it = _neurons.find(gid);
        if (it == _neurons.end()) {
            return {};
        }

        return &it->second.gpu;
    }

    rush::AABB<3, float> ComplexNeuronRepresentation::getSceneBoundingBox() const
    {
        return _sceneBoundingBox;
    }

    bool ComplexNeuronRepresentation::shouldDrawSegments() const
    {
        return _drawSegments;
    }

    void ComplexNeuronRepresentation::setDrawSegments(bool draw)
    {
        if (_drawSegments == draw) {
            return;
        }
        _drawSegments = draw;
        if (draw) {
            _render->getRoom()->markUsingModel(_segmentModel.get());
        } else {
            _render->getRoom()->unmarkUsingModel(_segmentModel.get());
        }
    }

    bool ComplexNeuronRepresentation::shouldDrawJoints() const
    {
        return _drawJoints;
    }

    void ComplexNeuronRepresentation::setDrawJoints(bool draw)
    {
        if (_drawJoints == draw) {
            return;
        }
        _drawJoints = draw;
        if (draw) {
            _render->getRoom()->markUsingModel(_jointModel.get());
        } else {
            _render->getRoom()->unmarkUsingModel(_jointModel.get());
        }
    }

    bool ComplexNeuronRepresentation::shouldDrawSomas() const
    {
        return _drawSomas;
    }

    void ComplexNeuronRepresentation::setDrawSomas(bool draw)
    {
        if (_drawSomas == draw) {
            return;
        }
        _drawSomas = draw;
        if (draw) {
            _render->getRoom()->markUsingModel(_somaModel.get());
        } else {
            _render->getRoom()->unmarkUsingModel(_somaModel.get());
        }
    }

    const std::shared_ptr<neon::ShaderUniformBuffer>& ComplexNeuronRepresentation::getUBO() const
    {
        return _ubo;
    }

    void ComplexNeuronRepresentation::refreshNeuronProperty(GID neuronId, const std::string& propertyName)
    {
        // TODO handle case where the neuron is still being processed.
        auto frame = _render->getApplication().getCurrentFrameInformation().currentFrame;

        if (auto it = _neurons.find(neuronId); it != _neurons.end()) {
            it->second.neuron.refreshProperty(propertyName);
            it->second.gpu.refreshGPUData(&it->second.neuron, frame);
        }

        if (propertyName == mindset::PROPERTY_TRANSFORM) {
            recalculateBoundingBox();
        }
    }

    void ComplexNeuronRepresentation::refreshData(const RepositoryView& view)
    {
        // Generate the preprocess task.
        auto& newGIDs = view.getNeurons();
        std::unordered_set set(newGIDs.begin(), newGIDs.end());

        // Remove
        bool erased = false;
        for (const auto& gid : _neuronsInDataset) {
            if (!set.contains(gid)) {
                erased = true;
                _neurons.erase(gid);
                _neuronProcessor.cancel(gid);
            }
        }

        // Add
        for (const auto& gid : set) {
            if (!_neuronsInDataset.contains(gid)) {
                if (auto optional = view.getRepository()->getNeuronAndDataset(gid)) {
                    auto [dataset, neuron] = *optional;
                    _neuronProcessor.process(gid, ComplexNeuronProcessData(gid, &dataset->getDataset(), neuron));
                }
            }
        }

        // Update all GPU neurons
        if (erased) {
            auto frame = _render->getApplication().getCurrentFrameInformation().currentFrame;
            for (auto& [neuron, gpu] : _neurons | std::views::values) {
                gpu.refreshGPUData(&neuron, frame);
            }
        }

        _neuronsInDataset = set;
    }

    void ComplexNeuronRepresentation::clearData()
    {
        _neuronProcessor.cancelAll();
        _neuronsInDataset.clear();
        _neurons.clear();
    }

    void ComplexNeuronRepresentation::addViewport(Viewport* viewport)
    {
        if (_viewports.contains(viewport)) {
            return;
        }
        auto fb = viewport->getInputFrameBuffer();
        Materials materials = {.segment = loadSegmentMaterial(viewport),
                               .joint = loadJointMaterial(viewport),
                               .soma = loadSomaMaterial(viewport)};

        for (auto& mesh : _segmentModel->getMeshes()) {
            mesh->getMaterials().insert(materials.segment);
        }
        for (auto& mesh : _jointModel->getMeshes()) {
            mesh->getMaterials().insert(materials.joint);
        }
        for (auto& mesh : _somaModel->getMeshes()) {
            mesh->getMaterials().insert(materials.soma);
        }

        _viewports.emplace(viewport, std::move(materials));
    }

    void ComplexNeuronRepresentation::removeViewport(Viewport* viewport)
    {
        auto it = _viewports.find(viewport);
        if (it == _viewports.end()) {
            return;
        }

        removeMaterials(it->second);

        _viewports.erase(it);
    }

    void ComplexNeuronRepresentation::setViewports(const std::unordered_set<Viewport*>& viewport)
    {
        // First, remove
        for (auto it = _viewports.begin(); it != _viewports.end();) {
            if (!viewport.contains(it->first)) {
                removeMaterials(it->second);
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

    size_t ComplexNeuronRepresentation::getTotalAllocatedMemory() const
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

    size_t ComplexNeuronRepresentation::getAllocatedInstanceMemory() const
    {
        size_t size = 0;
        for (auto& data : _segmentModel->getInstanceDatas()) {
            size += data->getBytesRequiredPerInstance() * data->getMaximumInstances();
        }
        for (auto& data : _jointModel->getInstanceDatas()) {
            size += data->getBytesRequiredPerInstance() * data->getMaximumInstances();
        }
        for (auto& data : _somaModel->getInstanceDatas()) {
            size += data->getBytesRequiredPerInstance() * data->getMaximumInstances();
        }
        return size;
    }

    size_t ComplexNeuronRepresentation::getUsedInstanceMemory() const
    {
        size_t size = 0;
        for (auto& data : _segmentModel->getInstanceDatas()) {
            size += data->getBytesRequiredPerInstance() * data->getInstanceAmount();
        }
        for (auto& data : _jointModel->getInstanceDatas()) {
            size += data->getBytesRequiredPerInstance() * data->getInstanceAmount();
        }
        for (auto& data : _somaModel->getInstanceDatas()) {
            size += data->getBytesRequiredPerInstance() * data->getInstanceAmount();
        }
        return size;
    }

    float ComplexNeuronRepresentation::getUsedInstanceMemoryPercentage() const
    {
        return static_cast<float>(getUsedInstanceMemory()) / static_cast<float>(getAllocatedInstanceMemory());
    }

    bool ComplexNeuronRepresentation::isWireframeMode() const
    {
        return _wireframe;
    }

    void ComplexNeuronRepresentation::setWireframeMode(bool wireframe)
    {
        if (wireframe == _wireframe) {
            return;
        }
        _wireframe = wireframe;

        recreateMaterials();
    }

    void ComplexNeuronRepresentation::reloadShader()
    {
        loadNeuronShader();
        loadJointShader();
        loadSomaShader();
        recreateMaterials();
    }
} // namespace neoneuron
