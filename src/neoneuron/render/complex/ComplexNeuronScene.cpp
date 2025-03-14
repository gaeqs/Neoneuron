//
// Created by gaeqs on 9/10/24.
//

#include "ComplexNeuronScene.h"

#include <neon/util/task/Coroutine.h>
#include <neoneuron/render/NeoneuronRender.h>

#include "ComplexNeuronSelector.h"

CMRC_DECLARE(resources);

namespace {
    neon::Coroutine<> deleteCoroutine(std::vector<std::shared_ptr<neon::Material>> materials) {
        for (size_t i = 0; i < 10; ++i) {
            co_yield neon::WaitForNextFrame();
        }
        materials.clear();
    }
}

namespace neoneuron {
    void ComplexNeuronScene::loadUniformBuffers() {
        auto* app = &_render->getApplication();

        std::vector bindings = {
            // GLOBAL DATA
            neon::ShaderUniformBinding::storageBuffer(sizeof(ComplexGPUNeuronGlobalData) * SOMA_INSTANCES),
            // SEGMENTS
            neon::ShaderUniformBinding::storageBuffer(sizeof(ComplexGPUNeuronSegment) * SEGMENT_INSTANCES),
            // JOINTS
            neon::ShaderUniformBinding::storageBuffer(sizeof(ComplexGPUNeuronJoint) * JOINT_INSTANCES),
            // SOMAS
            neon::ShaderUniformBinding::storageBuffer(sizeof(ComplexGPUNeuronSoma) * SOMA_INSTANCES),
            // SELECTION
            neon::ShaderUniformBinding::storageBuffer(sizeof(ComplexGPUNeuronSelectionData) * SEGMENT_INSTANCES),
            // SOMA GPU DATA
            neon::ShaderUniformBinding::storageBuffer(STORAGE_PER_SOMA * SOMA_INSTANCES),
            // SECTION GPU DATA
            neon::ShaderUniformBinding::storageBuffer(STORAGE_PER_SECTION * SEGMENT_INSTANCES),

            neon::ShaderUniformBinding::storageBuffer(MAX_SAVE_VERTICES * sizeof(rush::Vec4f),
                                                      neon::UniformBindingBufferType::SIMPLE),

            neon::ShaderUniformBinding::storageBuffer(MAX_SAVE_VERTICES * sizeof(rush::Vec4i),
                                                      neon::UniformBindingBufferType::SIMPLE),
        };

        _uboDescriptor = std::make_shared<neon::ShaderUniformDescriptor>(
            app, "neoneuron:complex_neuron_descriptor", bindings);


        _ubo = std::make_shared<neon::ShaderUniformBuffer>("neoneuron:complex_neuron_ubo", _uboDescriptor);

        std::vector slots = {
            neon::StorageBufferInstanceData::Slot(
                sizeof(ComplexGPUNeuronGlobalData),
                sizeof(ComplexGPUNeuronGlobalData),
                GLOBAL_DATA_BINDING,
                _ubo.get()
            ),
        };

        std::vector<std::type_index> types = {typeid(ComplexGPUNeuronGlobalData)};

        _globalInstanceData = std::make_shared<neon::StorageBufferInstanceData>(app, SOMA_INSTANCES, types, slots);
    }

    void ComplexNeuronScene::loadNeuronShader() {
        _neuronShader = std::make_shared<neon::ShaderProgram>(&_render->getApplication(), "Neuron");

        auto fs = cmrc::resources::get_filesystem();
        _neuronShader->addShader(neon::ShaderType::TASK, fs.open("/shader/neuron/complex/neuron.task"));
        _neuronShader->addShader(neon::ShaderType::MESH, fs.open("/shader/neuron/complex/neuron.mesh"));
        _neuronShader->addShader(neon::ShaderType::FRAGMENT, fs.open("/shader/neuron/complex/neuron.frag"));

        if (auto result = _neuronShader->compile(); result.has_value()) {
            neon::error() << result.value();
        }
    }

    void ComplexNeuronScene::loadJointShader() {
        _jointShader = std::make_shared<neon::ShaderProgram>(&_render->getApplication(), "Neuron");

        auto fs = cmrc::resources::get_filesystem();
        _jointShader->addShader(neon::ShaderType::TASK, fs.open("/shader/neuron/complex/joint.task"));
        _jointShader->addShader(neon::ShaderType::MESH, fs.open("/shader/neuron/complex/joint.mesh"));
        _jointShader->addShader(neon::ShaderType::FRAGMENT, fs.open("/shader/neuron/complex/joint.frag"));

        if (auto result = _jointShader->compile(); result.has_value()) {
            neon::error() << result.value();
        }
    }

    void ComplexNeuronScene::loadSomaShader() {
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

    void ComplexNeuronScene::loadNeuronMaterial() {
        auto* app = &_render->getApplication();
        neon::MaterialCreateInfo materialCreateInfo(_render->getRenderFrameBuffer(), _neuronShader);
        materialCreateInfo.rasterizer.cullMode = neon::CullMode::NONE;
        materialCreateInfo.rasterizer.polygonMode = _wireframe ? neon::PolygonMode::LINE : neon::PolygonMode::FILL;
        materialCreateInfo.descriptions.uniformBindings[UNIFORM_SET] = neon::DescriptorBinding::extra(_uboDescriptor);
        _neuronMaterial = std::make_shared<neon::Material>(app, "Neuron", materialCreateInfo);
    }

    void ComplexNeuronScene::loadJointMaterial() {
        auto* app = &_render->getApplication();
        neon::MaterialCreateInfo materialCreateInfo(_render->getRenderFrameBuffer(), _jointShader);
        materialCreateInfo.rasterizer.cullMode = neon::CullMode::NONE;
        materialCreateInfo.descriptions.uniformBindings[UNIFORM_SET] = neon::DescriptorBinding::extra(_uboDescriptor);
        materialCreateInfo.rasterizer.polygonMode = _wireframe ? neon::PolygonMode::LINE : neon::PolygonMode::FILL;
        _jointMaterial = std::make_shared<neon::Material>(app, "Joint", materialCreateInfo);
    }

    void ComplexNeuronScene::loadSomaMaterial() {
        auto* app = &_render->getApplication();
        neon::MaterialCreateInfo materialCreateInfo(_render->getRenderFrameBuffer(), _somaShader);
        materialCreateInfo.rasterizer.cullMode = neon::CullMode::NONE;
        materialCreateInfo.descriptions.uniformBindings[UNIFORM_SET] = neon::DescriptorBinding::extra(_uboDescriptor);
        materialCreateInfo.rasterizer.polygonMode = _wireframe ? neon::PolygonMode::LINE : neon::PolygonMode::FILL;
        _somaMaterial = std::make_shared<neon::Material>(app, "Soma", materialCreateInfo);
    }

    void ComplexNeuronScene::loadNeuronModel() {
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
        modelCreateInfo.instanceDataProvider = [this](neon::Application* app,
                                                      const neon::ModelCreateInfo& info,
                                                      const neon::Model* model) {
            std::vector indices = {
                neon::StorageBufferInstanceData::Slot(
                    sizeof(ComplexGPUNeuronSegment),
                    sizeof(ComplexGPUNeuronSegment),
                    SEGMENT_BINDING,
                    _ubo.get()
                )
            };
            return std::vector<neon::InstanceData*>{new neon::StorageBufferInstanceData(app, info, indices)};
        };

        _neuronModel = std::make_shared<neon::Model>(app, "Neuron", modelCreateInfo);
        _render->getRoom()->markUsingModel(_neuronModel.get());
    }

    void ComplexNeuronScene::loadJointModel() {
        constexpr size_t INSTANCES = 10000000;
        auto* app = &_render->getApplication();

        auto drawable = std::make_shared<neon::MeshShaderDrawable>(app, "Joint", _jointMaterial);
        drawable->setGroupsSupplier([](const neon::Model& model) {
            return rush::Vec<3, uint32_t>{model.getInstanceData(0)->getInstanceAmount(), 1, 1};
        });

        neon::ModelCreateInfo modelCreateInfo;
        modelCreateInfo.maximumInstances = INSTANCES;
        modelCreateInfo.drawables.push_back(drawable);
        modelCreateInfo.uniformBufferBindings[UNIFORM_SET] = neon::ModelBufferBinding::extra(_ubo);

        modelCreateInfo.defineInstanceType<ComplexGPUNeuronSegment>();
        modelCreateInfo.instanceDataProvider = [this](neon::Application* app,
                                                      const neon::ModelCreateInfo& info,
                                                      const neon::Model* model) {
            std::vector indices = {
                neon::StorageBufferInstanceData::Slot(
                    sizeof(ComplexGPUNeuronJoint),
                    sizeof(ComplexGPUNeuronJoint),
                    JOINT_BINDING,
                    _ubo.get()
                ),
            };
            return std::vector<neon::InstanceData*>{new neon::StorageBufferInstanceData(app, info, indices)};
        };

        _jointModel = std::make_shared<neon::Model>(app, "Joint", modelCreateInfo);
        _render->getRoom()->markUsingModel(_jointModel.get());
    }

    void ComplexNeuronScene::loadSomaModel() {
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
        modelCreateInfo.instanceDataProvider = [this](neon::Application* app,
                                                      const neon::ModelCreateInfo& info,
                                                      const neon::Model* model) {
            std::vector indices = {
                neon::StorageBufferInstanceData::Slot(
                    sizeof(ComplexGPUNeuronSoma),
                    sizeof(ComplexGPUNeuronSoma),
                    SOMA_BINDING,
                    _ubo.get()
                ),
            };
            return std::vector<neon::InstanceData*>{new neon::StorageBufferInstanceData(app, info, indices)};
        };

        _somaModel = std::make_shared<neon::Model>(app, "Soma", modelCreateInfo);
        _render->getRoom()->markUsingModel(_somaModel.get());
    }

    void ComplexNeuronScene::combineBoundingBoxes(const rush::AABB<3, float>& aabb) {
        auto min = rush::min(aabb.center - aabb.radius, _sceneBoundingBox.center - _sceneBoundingBox.radius);
        auto max = rush::max(aabb.center + aabb.radius, _sceneBoundingBox.center + _sceneBoundingBox.radius);
        _sceneBoundingBox = rush::AABB<3, float>::fromEdges(min, max);
    }

    void ComplexNeuronScene::recalculateBoundingBox() {
        if (_neurons.empty()) {
            _sceneBoundingBox = {};
            return;
        }

        auto bb = _neurons[0].getBoundingBox();
        auto min = bb.center - bb.radius;
        auto max = bb.center + bb.radius;

        for (size_t i = 1; i < _neurons.size(); i++) {
            bb = _neurons[i].getBoundingBox();
            min = rush::min(min, bb.center - bb.radius);
            max = rush::max(max, bb.center + bb.radius);
        }

        _sceneBoundingBox = rush::AABB<3, float>::fromEdges(min, max);
    }

    void ComplexNeuronScene::reassignMaterials() const {
        for (auto& mesh: _neuronModel->getMeshes()) {
            mesh->setMaterial(_neuronMaterial);
        }

        for (auto& mesh: _jointModel->getMeshes()) {
            mesh->setMaterial(_jointMaterial);
        }

        for (auto& mesh: _somaModel->getMeshes()) {
            mesh->setMaterial(_somaMaterial);
        }
    }

    ComplexNeuronScene::ComplexNeuronScene(NeoneuronRender* render) : _render(render), _wireframe(false) {
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
        _selector = ComplexNeuronSelector(this, _ubo.get(), SELECTION_BINDING);
    }

    ComplexNeuronScene::~ComplexNeuronScene() {
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

    NeoneuronRender* ComplexNeuronScene::getRender() {
        return _render;
    }

    const NeoneuronRender* ComplexNeuronScene::getRender() const {
        return _render;
    }

    const std::vector<ComplexNeuron>& ComplexNeuronScene::getNeurons() const {
        return _neurons;
    }

    AbstractSelector& ComplexNeuronScene::getSelector() {
        return _selector;
    }

    const AbstractSelector& ComplexNeuronScene::getSelector() const {
        return _selector;
    }

    size_t ComplexNeuronScene::getNeuronsAmount() {
        return _neurons.size();
    }

    size_t ComplexNeuronScene::getSectionsAmount() {
        return _neuronModel->getInstanceData(0)->getInstanceAmount();
    }

    size_t ComplexNeuronScene::getJointsAmount() {
        return _jointModel->getInstanceData(0)->getInstanceAmount();
    }

    size_t ComplexNeuronScene::getSomasAmount() {
        return _somaModel->getInstanceData(0)->getInstanceAmount();
    }

    mnemea::Dataset& ComplexNeuronScene::getDataset() {
        return _dataset;
    }

    const mnemea::Dataset& ComplexNeuronScene::getDataset() const {
        return _dataset;
    }


    std::optional<mnemea::Neuron*> ComplexNeuronScene::findPrototypeNeuron(mnemea::UID uid) {
        return _dataset.getNeuron(uid);
    }

    std::optional<const mnemea::Neuron*> ComplexNeuronScene::findPrototypeNeuron(mnemea::UID uid) const {
        return _dataset.getNeuron(uid);
    }

    std::optional<ComplexNeuron*> ComplexNeuronScene::findNeuron(mnemea::UID uid) {
        auto it = std::find_if(
            _neurons.begin(), _neurons.end(),
            [uid](const ComplexNeuron& neuron) {
                return uid == neuron.getUID();
            }
        );
        if (it == _neurons.end()) return {};
        return {&*it};
    }

    std::optional<const ComplexNeuron*> ComplexNeuronScene::findNeuron(mnemea::UID uid) const {
        auto it = std::find_if(
            _neurons.cbegin(), _neurons.cend(),
            [uid](const ComplexNeuron& neuron) {
                return uid == neuron.getUID();
            }
        );
        if (it == _neurons.cend()) return {};
        return {&*it};
    }

    std::optional<ComplexGPUNeuron*> ComplexNeuronScene::findGPUNeuron(mnemea::UID uid) {
        auto it = std::find_if(
            _neurons.cbegin(), _neurons.cend(),
            [uid](const ComplexNeuron& neuron) {
                return uid == neuron.getUID();
            }
        );
        if (it == _neurons.cend()) return {};
        ptrdiff_t index = it - _neurons.begin();

        return &_gpuNeurons[index];
    }

    std::optional<const ComplexGPUNeuron*> ComplexNeuronScene::findGPUNeuron(mnemea::UID uid) const {
        auto it = std::find_if(
            _neurons.cbegin(), _neurons.cend(),
            [uid](const ComplexNeuron& neuron) {
                return uid == neuron.getUID();
            }
        );
        if (it == _neurons.cend()) return {};
        ptrdiff_t index = it - _neurons.begin();

        return &_gpuNeurons[index];
    }

    bool ComplexNeuronScene::addNeuron(const mnemea::Neuron& neuron) {
        auto [it, inserted] = _dataset.addNeuron(neuron);
        if (!inserted) return false;
        auto result = ComplexNeuron(&_dataset, it);
        auto bb = result.getBoundingBox();
        _neurons.push_back(std::move(result));
        _gpuNeurons.emplace_back(
            _globalInstanceData,
            _neuronModel, _jointModel, _somaModel,
            0, 0, 0,
            &_neurons.back(),
            _render->getApplication().getCurrentFrameInformation().currentFrame
        );
        if (_neurons.size() == 1) {
            _sceneBoundingBox = bb;
        } else {
            combineBoundingBoxes(bb);
        }
        return true;
    }

    bool ComplexNeuronScene::addNeuron(mnemea::Neuron&& neuron) {
        auto [it, inserted] = _dataset.addNeuron(neuron);
        if (!inserted) return false;
        auto result = ComplexNeuron(&_dataset, it);
        auto bb = result.getBoundingBox();
        _neurons.push_back(std::move(result));
        _gpuNeurons.emplace_back(
            _globalInstanceData,
            _neuronModel, _jointModel, _somaModel,
            0, 0, 0,
            &_neurons.back(),
            _render->getApplication().getCurrentFrameInformation().currentFrame
        );
        if (_neurons.size() == 1) {
            _sceneBoundingBox = bb;
        } else {
            combineBoundingBoxes(bb);
        }
        return true;
    }

    bool ComplexNeuronScene::removeNeuron(mnemea::UID neuronId) {
        auto it = std::find_if(
            _neurons.begin(),
            _neurons.end(),
            [&neuronId](const ComplexNeuron& neuron) {
                return neuron.getUID() == neuronId;
            }
        );

        if (it == _neurons.end()) return false;
        ptrdiff_t index = it - _neurons.begin();
        _neurons.erase(_neurons.begin() + index);
        _gpuNeurons.erase(_gpuNeurons.begin() + index);
        _dataset.removeNeuron(neuronId);

        auto frame = _render->getApplication().getCurrentFrameInformation().currentFrame;
        for (auto& neuron: _neurons) {
            if (auto gpuNeuron = findGPUNeuron(neuron.getUID()); gpuNeuron.has_value()) {
                gpuNeuron.value()->refreshGPUData(&neuron, frame);
            }
        }

        _selector.clearSelection();
        //_selector.refreshGPUData();

        recalculateBoundingBox();

        return true;
    }

    rush::AABB<3, float> ComplexNeuronScene::getSceneBoundingBox() const {
        return _sceneBoundingBox;
    }

    const std::shared_ptr<neon::ShaderUniformBuffer>& ComplexNeuronScene::getUBO() const {
        return _ubo;
    }

    void ComplexNeuronScene::refreshNeuronProperty(mnemea::UID neuronId, const std::string& propertyName) {
        auto frame = _render->getApplication().getCurrentFrameInformation().currentFrame;
        if (auto neuron = findNeuron(neuronId); neuron.has_value()) {
            neuron.value()->refreshProperty(propertyName);
            if (auto gpuNeuron = findGPUNeuron(neuronId); gpuNeuron.has_value()) {
                gpuNeuron.value()->refreshProperty(neuron.value(), frame, propertyName);
            }
        }


        if (propertyName == mnemea::PROPERTY_TRANSFORM) {
            recalculateBoundingBox();
        }
    }

    mnemea::UID ComplexNeuronScene::findAvailableUID() const {
        std::unordered_set<mnemea::UID> uids;
        for (auto& neuron: _neurons) {
            uids.insert(neuron.getUID());
        }

        size_t smallest = 0;

        while (uids.contains(smallest)) {
            ++smallest;
        }

        return smallest;
    }

    bool ComplexNeuronScene::isWireframeMode() const {
        return _wireframe;
    }

    void ComplexNeuronScene::setWireframeMode(bool wireframe) {
        if (wireframe == _wireframe) return;
        _wireframe = wireframe;

        std::vector materials = {_neuronMaterial, _jointMaterial, _somaMaterial};

        loadNeuronMaterial();
        loadJointMaterial();
        loadSomaMaterial();
        reassignMaterials();

        _render->getApplication().getTaskRunner().launchCoroutine(deleteCoroutine(std::move(materials)));
    }

    void ComplexNeuronScene::reloadShader() {
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

    void ComplexNeuronScene::checkForNewNeurons() {
        for (auto& neuron: _dataset.getNeurons()) {
            if (findNeuron(neuron.getUID()).has_value()) continue;
            std::cout << "Computing neuron " << neuron.getUID() << std::endl;
            auto result = ComplexNeuron(&_dataset, &neuron);
            auto bb = result.getBoundingBox();
            _neurons.push_back(std::move(result));
            _gpuNeurons.emplace_back(
                _globalInstanceData,
                _neuronModel, _jointModel, _somaModel,
                0, 0, 0,
                &_neurons.back(),
                _render->getApplication().getCurrentFrameInformation().currentFrame
            );
            if (_neurons.size() == 1) {
                _sceneBoundingBox = bb;
            } else {
                combineBoundingBoxes(bb);
            }
        }
    }
}
