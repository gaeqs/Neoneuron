//
// Created by gaeqs on 9/10/24.
//

#include "ComplexNeuronScene.h"

#include <neoneuron/render/NeoneuronRender.h>

#include "ComplexNeuronSelector.h"

CMRC_DECLARE(resources);

namespace neoneuron {
    void ComplexNeuronScene::loadUniformBuffers() {
        constexpr size_t INSTANCES = 10000000;


        auto* app = &_render->getApplication();

        std::vector bindings = {
            // SEGMENTS
            neon::ShaderUniformBinding::storageBuffer(sizeof(ComplexGPUNeuronSegment) * INSTANCES),
            // JOINTS
            neon::ShaderUniformBinding::storageBuffer(sizeof(ComplexGPUNeuronJoint) * INSTANCES),
            // SELECTION
            neon::ShaderUniformBinding::storageBuffer(sizeof(ComplexGPUNeuronSelectionData) * INSTANCES)
        };

        _uboDescriptor = std::make_shared<neon::ShaderUniformDescriptor>(
            app, "neoneuron:complex_neuron_descriptor", bindings);


        _ubo = std::make_shared<neon::ShaderUniformBuffer>("neoneuron:complex_neuron_ubo", _uboDescriptor);
    }

    void ComplexNeuronScene::loadNeuronModel() {
        constexpr size_t INSTANCES = 10000000;
        auto* app = &_render->getApplication();

        auto shader = std::make_shared<neon::ShaderProgram>(&_render->getApplication(), "Neuron");

        auto fs = cmrc::resources::get_filesystem();
        shader->addShader(neon::ShaderType::TASK, fs.open("/shader/neuron/complex/neuron.task"));
        shader->addShader(neon::ShaderType::MESH, fs.open("/shader/neuron/complex/neuron.mesh"));
        shader->addShader(neon::ShaderType::FRAGMENT, fs.open("/shader/neuron/complex/neuron.frag"));

        if (auto result = shader->compile(); result.has_value()) {
            _render->getApplication().getLogger().error(result.value());
            return;
        }

        neon::MaterialCreateInfo materialCreateInfo(_render->getRenderFrameBuffer(), shader);
        materialCreateInfo.rasterizer.cullMode = neon::CullMode::NONE;
        materialCreateInfo.descriptions.uniformBindings[2] = neon::DescriptorBinding::extra(_uboDescriptor);
        auto material = std::make_shared<neon::Material>(app, "Neuron", materialCreateInfo);

        auto drawable = std::make_shared<neon::MeshShaderDrawable>(app, "Neuron", material);
        drawable->setGroupsSupplier([](const neon::Model& model) {
            return rush::Vec<3, uint32_t>{model.getInstanceData(0)->getInstanceAmount(), 1, 1};
        });

        neon::ModelCreateInfo modelCreateInfo;
        modelCreateInfo.maximumInstances = INSTANCES;
        modelCreateInfo.drawables.push_back(drawable);
        modelCreateInfo.uniformBufferBindings[2] = neon::ModelBufferBinding::extra(_ubo);

        modelCreateInfo.defineInstanceType<ComplexGPUNeuronSegment>();
        modelCreateInfo.instanceDataProvider = [this](neon::Application* app,
                                                      const neon::ModelCreateInfo& info,
                                                      const neon::Model* model) {
            std::vector indices = {
                neon::StorageBufferInstanceData::Slot(
                    sizeof(ComplexGPUNeuronSegment),
                    sizeof(ComplexGPUNeuronSegment),
                    0,
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

        auto shader = std::make_shared<neon::ShaderProgram>(&_render->getApplication(), "Neuron");

        auto fs = cmrc::resources::get_filesystem();
        shader->addShader(neon::ShaderType::TASK, fs.open("/shader/neuron/complex/joint.task"));
        shader->addShader(neon::ShaderType::MESH, fs.open("/shader/neuron/complex/joint.mesh"));
        shader->addShader(neon::ShaderType::FRAGMENT, fs.open("/shader/neuron/complex/joint.frag"));

        if (auto result = shader->compile(); result.has_value()) {
            _render->getApplication().getLogger().error(result.value());
            return;
        }

        neon::MaterialCreateInfo materialCreateInfo(_render->getRenderFrameBuffer(), shader);
        materialCreateInfo.rasterizer.cullMode = neon::CullMode::NONE;
        materialCreateInfo.descriptions.uniformBindings[2] = neon::DescriptorBinding::extra(_uboDescriptor);
        auto material = std::make_shared<neon::Material>(app, "Joint", materialCreateInfo);

        auto drawable = std::make_shared<neon::MeshShaderDrawable>(app, "Joint", material);
        drawable->setGroupsSupplier([](const neon::Model& model) {
            return rush::Vec<3, uint32_t>{model.getInstanceData(0)->getInstanceAmount(), 1, 1};
        });

        neon::ModelCreateInfo modelCreateInfo;
        modelCreateInfo.maximumInstances = INSTANCES;
        modelCreateInfo.drawables.push_back(drawable);
        modelCreateInfo.uniformBufferBindings[2] = neon::ModelBufferBinding::extra(_ubo);

        modelCreateInfo.defineInstanceType<ComplexGPUNeuronSegment>();
        modelCreateInfo.instanceDataProvider = [this](neon::Application* app,
                                                      const neon::ModelCreateInfo& info,
                                                      const neon::Model* model) {
            std::vector indices = {
                neon::StorageBufferInstanceData::Slot(
                    sizeof(ComplexGPUNeuronJoint),
                    sizeof(ComplexGPUNeuronJoint),
                    1,
                    _ubo.get()
                ),
            };
            return std::vector<neon::InstanceData*>{new neon::StorageBufferInstanceData(app, info, indices)};
        };

        _jointModel = std::make_shared<neon::Model>(app, "Joint", modelCreateInfo);
        _render->getRoom()->markUsingModel(_jointModel.get());
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

    ComplexNeuronScene::ComplexNeuronScene(NeoneuronRender* render) : _render(render) {
        loadUniformBuffers();
        loadNeuronModel();
        loadJointModel();
        _selector = ComplexNeuronSelector(this, _ubo.get(), 2);
    }

    ComplexNeuronScene::~ComplexNeuronScene() {
        if (_neuronModel != nullptr) {
            _render->getRoom()->unmarkUsingModel(_neuronModel.get());
        }
        if (_jointModel != nullptr) {
            _render->getRoom()->unmarkUsingModel(_jointModel.get());
        }
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

    std::optional<ComplexNeuron*> ComplexNeuronScene::findNeuron(UID uid) {
        auto it = std::find_if(
            _neurons.begin(), _neurons.end(),
            [uid](const ComplexNeuron& neuron) {
                return uid == neuron.getId();
            }
        );
        if (it == _neurons.end()) return {};
        return {&*it};
    }

    std::optional<const ComplexNeuron*> ComplexNeuronScene::findNeuron(UID uid) const {
        auto it = std::find_if(
            _neurons.cbegin(), _neurons.cend(),
            [uid](const ComplexNeuron& neuron) {
                return uid == neuron.getId();
            }
        );
        if (it == _neurons.cend()) return {};
        return {&*it};
    }

    std::optional<ComplexGPUNeuron*> ComplexNeuronScene::findGPUNeuron(UID uid) {
        auto it = std::find_if(
            _neurons.cbegin(), _neurons.cend(),
            [uid](const ComplexNeuron& neuron) {
                return uid == neuron.getId();
            }
        );
        if (it == _neurons.cend()) return {};
        ptrdiff_t index = it - _neurons.begin();

        return &_gpuNeurons[index];
    }

    std::optional<const ComplexGPUNeuron*> ComplexNeuronScene::findGPUNeuron(UID uid) const {
        auto it = std::find_if(
            _neurons.cbegin(), _neurons.cend(),
            [uid](const ComplexNeuron& neuron) {
                return uid == neuron.getId();
            }
        );
        if (it == _neurons.cend()) return {};
        ptrdiff_t index = it - _neurons.begin();

        return &_gpuNeurons[index];
    }

    bool ComplexNeuronScene::addNeuron(const PrototypeNeuron& neuron) {
        auto result = ComplexNeuron::fromPrototype(neuron);
        if (result.isOk()) {
            addNeuron(std::move(result.getResult()));
            return true;
        }

        neon::Logger::defaultLogger()->error(result.getError());
        return false;
    }

    void ComplexNeuronScene::addNeuron(const ComplexNeuron& neuron) {
        _neurons.push_back(neuron);
        _gpuNeurons.emplace_back(_neuronModel, _jointModel, 0, 0, &_neurons.back());

        if (_neurons.size() == 1) {
            _sceneBoundingBox = neuron.getBoundingBox();
        } else {
            combineBoundingBoxes(neuron.getBoundingBox());
        }
    }

    void ComplexNeuronScene::addNeuron(ComplexNeuron&& neuron) {
        auto bb = neuron.getBoundingBox();
        _neurons.push_back(std::move(neuron));
        _gpuNeurons.emplace_back(_neuronModel, _jointModel, 0, 0, &_neurons.back());
        if (_neurons.size() == 1) {
            _sceneBoundingBox = bb;
        } else {
            combineBoundingBoxes(bb);
        }
    }

    bool ComplexNeuronScene::removeNeuron(UID neuronId) {
        auto it = std::find_if(
            _neurons.begin(),
            _neurons.end(),
            [&neuronId](const ComplexNeuron& neuron) {
                return neuron.getId() == neuronId;
            }
        );

        if (it == _neurons.end()) return false;
        ptrdiff_t index = it - _neurons.begin();
        _neurons.erase(_neurons.begin() + index);
        _gpuNeurons.erase(_gpuNeurons.begin() + index);
        for (auto& gpuNeuron: _gpuNeurons) {
            gpuNeuron.refreshGPUData();
        }

        _selector.refreshGPUData();

        recalculateBoundingBox();

        return true;
    }

    rush::AABB<3, float> ComplexNeuronScene::getSceneBoundingBox() const {
        return _sceneBoundingBox;
    }
}
