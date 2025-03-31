//
// Created by gaeqs on 9/10/24.
//

#include "ComplexNeuronRepresentation.h"

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

        _globalInstanceData = std::make_shared<neon::StorageBufferInstanceData>(app, SOMA_INSTANCES, types, slots);
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

    void ComplexNeuronRepresentation::loadNeuronMaterial()
    {
        auto* app = &_render->getApplication();
        neon::MaterialCreateInfo materialCreateInfo(_render->getRenderFrameBuffer(), _neuronShader);
        materialCreateInfo.rasterizer.cullMode = neon::CullMode::BACK;
        materialCreateInfo.rasterizer.polygonMode = _wireframe ? neon::PolygonMode::LINE : neon::PolygonMode::FILL;
        materialCreateInfo.descriptions.uniformBindings[UNIFORM_SET] = neon::DescriptorBinding::extra(_uboDescriptor);
        _neuronMaterial = std::make_shared<neon::Material>(app, "Neuron", materialCreateInfo);
    }

    void ComplexNeuronRepresentation::loadJointMaterial()
    {
        auto* app = &_render->getApplication();
        neon::MaterialCreateInfo materialCreateInfo(_render->getRenderFrameBuffer(), _jointShader);
        materialCreateInfo.rasterizer.cullMode = neon::CullMode::BACK;
        materialCreateInfo.descriptions.uniformBindings[UNIFORM_SET] = neon::DescriptorBinding::extra(_uboDescriptor);
        materialCreateInfo.rasterizer.polygonMode = _wireframe ? neon::PolygonMode::LINE : neon::PolygonMode::FILL;
        _jointMaterial = std::make_shared<neon::Material>(app, "Joint", materialCreateInfo);
    }

    void ComplexNeuronRepresentation::loadSomaMaterial()
    {
        auto* app = &_render->getApplication();
        neon::MaterialCreateInfo materialCreateInfo(_render->getRenderFrameBuffer(), _somaShader);
        materialCreateInfo.rasterizer.cullMode = neon::CullMode::BACK;
        materialCreateInfo.descriptions.uniformBindings[UNIFORM_SET] = neon::DescriptorBinding::extra(_uboDescriptor);
        materialCreateInfo.rasterizer.polygonMode = _wireframe ? neon::PolygonMode::LINE : neon::PolygonMode::FILL;
        _somaMaterial = std::make_shared<neon::Material>(app, "Soma", materialCreateInfo);
    }

    void ComplexNeuronRepresentation::loadNeuronModel()
    {
        constexpr size_t INSTANCES = 10000000;
        auto* app = &_render->getApplication();

        auto drawable = std::make_shared<neon::MeshShaderDrawable>(app, "Neuron", _neuronMaterial);
        drawable->setGroupsSupplier([](const neon::Model& model) {
            uint32_t amount = model.getInstanceData(0)->getInstanceAmount();
            uint32_t tasks = amount / 32 + (amount % 32 != 0);
            return rush::Vec<3, uint32_t>{tasks, 1, 1};
        });

        neon::ModelCreateInfo modelCreateInfo;
        modelCreateInfo.maximumInstances = INSTANCES;
        modelCreateInfo.drawables.push_back(drawable);
        modelCreateInfo.uniformBufferBindings[UNIFORM_SET] = neon::ModelBufferBinding::extra(_ubo);

        modelCreateInfo.defineInstanceType<ComplexGPUNeuronSegment>();
        modelCreateInfo.instanceDataProvider = [this](neon::Application* app, const neon::ModelCreateInfo& info,
                                                      const neon::Model* model) {
            std::vector indices = {neon::StorageBufferInstanceData::Slot(
                sizeof(ComplexGPUNeuronSegment), sizeof(ComplexGPUNeuronSegment), SEGMENT_BINDING, _ubo.get())};
            return std::vector<neon::InstanceData*>{new neon::StorageBufferInstanceData(app, info, indices)};
        };

        _neuronModel = std::make_shared<neon::Model>(app, "Neuron", modelCreateInfo);
        if (_drawSegments) {
            _render->getRoom()->markUsingModel(_neuronModel.get());
        }
    }

    void ComplexNeuronRepresentation::loadJointModel()
    {
        constexpr size_t INSTANCES = 10000000;
        auto* app = &_render->getApplication();

        auto drawable = std::make_shared<neon::MeshShaderDrawable>(app, "Joint", _jointMaterial);
        drawable->setGroupsSupplier([](const neon::Model& model) {
            uint32_t amount = model.getInstanceData(0)->getInstanceAmount();
            uint32_t tasks = amount / 32 + (amount % 32 != 0);
            return rush::Vec<3, uint32_t>{tasks, 1, 1};
        });

        neon::ModelCreateInfo modelCreateInfo;
        modelCreateInfo.maximumInstances = INSTANCES;
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
        constexpr size_t INSTANCES = 100000;
        auto* app = &_render->getApplication();

        auto drawable = std::make_shared<neon::MeshShaderDrawable>(app, "Soma", _somaMaterial);
        drawable->setGroupsSupplier([](const neon::Model& model) {
            return rush::Vec<3, uint32_t>{model.getInstanceData(0)->getInstanceAmount(), 1, 1};
        });

        neon::ModelCreateInfo modelCreateInfo;
        modelCreateInfo.maximumInstances = INSTANCES;
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

        auto bb = it->second.getBoundingBox();
        auto min = bb.center - bb.radius;
        auto max = bb.center + bb.radius;

        for (; it != end; ++it) {
            bb = it->second.getBoundingBox();
            min = rush::min(min, bb.center - bb.radius);
            max = rush::max(max, bb.center + bb.radius);
        }

        _sceneBoundingBox = rush::AABB<3, float>::fromEdges(min, max);
    }

    void ComplexNeuronRepresentation::reassignMaterials() const
    {
        for (auto& mesh : _neuronModel->getMeshes()) {
            mesh->setMaterial(_neuronMaterial);
        }

        for (auto& mesh : _jointModel->getMeshes()) {
            mesh->setMaterial(_jointMaterial);
        }

        for (auto& mesh : _somaModel->getMeshes()) {
            mesh->setMaterial(_somaMaterial);
        }
    }
    void ComplexNeuronRepresentation::onNeuronAdded(mindset::Neuron* neuron)
    {
        if (findNeuron(neuron->getUID()).has_value()) {
            onNeuronRemoved(neuron->getUID());
        }

        auto& runner = _render->getApplication().getTaskRunner();
        runner.executeAsync(
            [](ComplexNeuronRepresentation* scene, mindset::Neuron* neuron) {
                auto& dataset = scene->_render->getNeoneuronApplication()->getDataset();
                auto complex = ComplexNeuron(&dataset, neuron);
                scene->_render->getApplication().getTaskRunner().executeOnMainThread(
                    [](ComplexNeuronRepresentation* scene, ComplexNeuron c) { scene->addComplexNeuron(std::move(c)); },
                    scene, std::move(complex));
            },
            this, neuron);
    }
    void ComplexNeuronRepresentation::onNeuronRemoved(mindset::UID uid)
    {
        if (_neurons.erase(uid) == 0) {
            return;
        }

        _gpuNeurons.erase(uid);

        auto frame = _render->getApplication().getCurrentFrameInformation().currentFrame;
        for (auto& neuron : _neurons | std::views::values) {
            if (auto gpuNeuron = findGPUNeuron(neuron.getUID()); gpuNeuron.has_value()) {
                gpuNeuron.value()->refreshGPUData(&neuron, frame);
            }
        }

        updateGPURepresentationData();
    }

    void ComplexNeuronRepresentation::addComplexNeuron(ComplexNeuron&& complex)
    {
        auto uid = complex.getUID();
        auto bb = complex.getBoundingBox();
        auto currentFrame = _render->getApplication().getCurrentFrameInformation().currentFrame;

        auto [it, ok] = _neurons.emplace(uid, std::move(complex));
        if (!ok) {
            return;
        }
        _gpuNeurons.emplace(std::piecewise_construct, std::forward_as_tuple(uid),
                            std::forward_as_tuple(_globalInstanceData, _neuronModel, _jointModel, _somaModel, 0, 0, 0,
                                                  &it->second, currentFrame));
        if (_neurons.size() == 1) {
            _sceneBoundingBox = bb;
        } else {
            combineBoundingBoxes(bb);
        }

        updateGPURepresentationData();
    }
    void ComplexNeuronRepresentation::onClear()
    {
        _neurons.clear();
        _gpuNeurons.clear();
        _sceneBoundingBox = {};

        updateGPURepresentationData();
    }
    void ComplexNeuronRepresentation::onSelectionEvent(SelectionEvent event)
    {
        if (!event.segmentsChanged) {
            return;
        }

        auto data = static_cast<ComplexGPUSelectionData*>(_ubo->fetchData(SELECTION_BINDING));

        for (uint32_t id : _selection) {
            (data + id)->selected = false;
        }

        _selection.clear();

        for (auto& [neuronId, segmentId] : event.selector->getSelectedSections()) {
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
    void ComplexNeuronRepresentation::updateGPURepresentationData()
    {
        _ubo->uploadData(REPRESENTATION_BINDING,
                         ComplexNeuronRepresentationData(getSectionsAmount(), getJointsAmount()));
    }

    ComplexNeuronRepresentation::ComplexNeuronRepresentation(NeoneuronRender* render) :
        _render(render),
        _wireframe(false),
        _drawSegments(true),
        _drawJoints(true),
        _drawSomas(true)
    {
        loadUniformBuffers();
        loadNeuronShader();
        loadJointShader();
        loadSomaShader();
        loadNeuronMaterial();
        loadJointMaterial();
        loadSomaMaterial();
        loadNeuronModel();
        loadJointModel();
        loadSomaModel();

        _neuronAddListener = [this](mindset::Neuron* neuron) { onNeuronAdded(neuron); };
        _neuronRemoveListener = [this](mindset::UID uid) { onNeuronRemoved(uid); };
        _clearListener = [this](void*) { onClear(); };
        _selectionListener = [this](SelectionEvent event) { onSelectionEvent(event); };

        auto& dataset = render->getNeoneuronApplication()->getDataset();
        dataset.getNeuronAddedEvent() += _neuronAddListener;
        dataset.getNeuronRemovedEvent() += _neuronRemoveListener;
        dataset.getClearEvent() += _clearListener;
        render->getNeoneuronApplication()->getSelector().onSelectionEvent() += _selectionListener;
    }

    ComplexNeuronRepresentation::~ComplexNeuronRepresentation()
    {
        if (_neuronModel != nullptr) {
            _render->getRoom()->unmarkUsingModel(_neuronModel.get());
        }
        if (_jointModel != nullptr) {
            _render->getRoom()->unmarkUsingModel(_jointModel.get());
        }
        if (_somaModel != nullptr) {
            _render->getRoom()->unmarkUsingModel(_jointModel.get());
        }
    }

    NeoneuronRender* ComplexNeuronRepresentation::getRender()
    {
        return _render;
    }

    const NeoneuronRender* ComplexNeuronRepresentation::getRender() const
    {
        return _render;
    }

    const std::unordered_map<mindset::UID, ComplexNeuron>& ComplexNeuronRepresentation::getNeurons() const
    {
        return _neurons;
    }

    size_t ComplexNeuronRepresentation::getNeuronsAmount()
    {
        return _neurons.size();
    }

    size_t ComplexNeuronRepresentation::getSectionsAmount()
    {
        return _neuronModel->getInstanceData(0)->getInstanceAmount();
    }

    size_t ComplexNeuronRepresentation::getJointsAmount()
    {
        return _jointModel->getInstanceData(0)->getInstanceAmount();
    }

    size_t ComplexNeuronRepresentation::getSomasAmount()
    {
        return _somaModel->getInstanceData(0)->getInstanceAmount();
    }

    std::optional<ComplexNeuron*> ComplexNeuronRepresentation::findNeuron(mindset::UID uid)
    {
        auto it = _neurons.find(uid);
        if (it == _neurons.end()) {
            return {};
        }

        return &it->second;
    }

    std::optional<const ComplexNeuron*> ComplexNeuronRepresentation::findNeuron(mindset::UID uid) const
    {
        auto it = _neurons.find(uid);
        if (it == _neurons.end()) {
            return {};
        }

        return &it->second;
    }

    std::optional<ComplexGPUNeuron*> ComplexNeuronRepresentation::findGPUNeuron(mindset::UID uid)
    {
        auto it = _gpuNeurons.find(uid);
        if (it == _gpuNeurons.end()) {
            return {};
        }

        return &it->second;
    }

    std::optional<const ComplexGPUNeuron*> ComplexNeuronRepresentation::findGPUNeuron(mindset::UID uid) const
    {
        auto it = _gpuNeurons.find(uid);
        if (it == _gpuNeurons.end()) {
            return {};
        }

        return &it->second;
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
            _render->getRoom()->markUsingModel(_neuronModel.get());
        } else {
            _render->getRoom()->unmarkUsingModel(_neuronModel.get());
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

    void ComplexNeuronRepresentation::refreshNeuronProperty(mindset::UID neuronId, const std::string& propertyName)
    {
        auto frame = _render->getApplication().getCurrentFrameInformation().currentFrame;
        if (auto neuron = findNeuron(neuronId); neuron.has_value()) {
            neuron.value()->refreshProperty(propertyName);
            if (auto gpuNeuron = findGPUNeuron(neuronId); gpuNeuron.has_value()) {
                gpuNeuron.value()->refreshProperty(neuron.value(), frame, propertyName);
            }
        }

        if (propertyName == mindset::PROPERTY_TRANSFORM) {
            recalculateBoundingBox();
        }
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

        std::vector materials = {_neuronMaterial, _jointMaterial, _somaMaterial};

        loadNeuronMaterial();
        loadJointMaterial();
        loadSomaMaterial();
        reassignMaterials();

        _render->getApplication().getTaskRunner().launchCoroutine(deleteCoroutine(std::move(materials)));
    }

    void ComplexNeuronRepresentation::reloadShader()
    {
        std::vector materials = {_neuronMaterial, _jointMaterial, _somaMaterial};

        loadNeuronShader();
        loadNeuronMaterial();
        loadJointShader();
        loadJointMaterial();
        loadSomaShader();
        loadSomaMaterial();
        reassignMaterials();

        _render->getApplication().getTaskRunner().launchCoroutine(deleteCoroutine(std::move(materials)));
    }
} // namespace neoneuron
