//
// Created by gaeqs on 9/10/24.
//

#include "NeuronScene.h"

#include <neoneuron/render/NeoneuronRender.h>

#include "NeuronSelector.h"

CMRC_DECLARE(resources);

namespace neoneuron {
    void NeuronScene::combineBoundingBoxes(const rush::AABB<3, float>& aabb) {
        auto min = rush::min(aabb.center - aabb.radius, _sceneBoundingBox.center - _sceneBoundingBox.radius);
        auto max = rush::max(aabb.center + aabb.radius, _sceneBoundingBox.center + _sceneBoundingBox.radius);
        _sceneBoundingBox = rush::AABB<3, float>::fromEdges(min, max);
    }

    void NeuronScene::recalculateBoundingBox() {
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

    NeuronScene::NeuronScene(NeoneuronRender* render) : _render(render) {
        constexpr size_t INSTANCES = 10000000;

        auto* app = &render->getApplication();

        auto shader = std::make_shared<neon::ShaderProgram>(&render->getApplication(), "Neuron");

        auto fs = cmrc::resources::get_filesystem();
        shader->addShader(neon::ShaderType::TASK, fs.open("/shader/neuron/neuron.task"));
        shader->addShader(neon::ShaderType::MESH, fs.open("/shader/neuron/neuron.mesh"));
        shader->addShader(neon::ShaderType::FRAGMENT, fs.open("/shader/neuron/neuron.frag"));

        if (auto result = shader->compile(); result.has_value()) {
            render->getApplication().getLogger().error(result.value());
            return;
        }

        std::vector modelBindings = {
            neon::ShaderUniformBinding::storageBuffer(sizeof(GPUNeuronSegment) * INSTANCES),
            neon::ShaderUniformBinding::storageBuffer(sizeof(GPUNeuronSelectionData) * INSTANCES)
        };
        auto modelDescriptor = std::make_shared<neon::ShaderUniformDescriptor>(app, "model", modelBindings);

        neon::MaterialCreateInfo materialCreateInfo(render->getRenderFrameBuffer(), shader);
        materialCreateInfo.rasterizer.cullMode = neon::CullMode::NONE;
        materialCreateInfo.descriptions.extraUniforms.push_back(modelDescriptor);
        auto material = std::make_shared<neon::Material>(app, "Neuron", materialCreateInfo);

        auto drawable = std::make_shared<neon::MeshShaderDrawable>(app, "Neuron", material);
        drawable->setGroupsSupplier([](const neon::Model& model) {
            return rush::Vec<3, uint32_t>{model.getInstanceData(0)->getInstanceAmount(), 1, 1};
        });

        neon::ModelCreateInfo modelCreateInfo;
        modelCreateInfo.maximumInstances = 10000000;
        modelCreateInfo.drawables.push_back(drawable);
        modelCreateInfo.uniformDescriptor = modelDescriptor;

        modelCreateInfo.defineInstanceType<GPUNeuronSegment>();
        modelCreateInfo.instanceDataProvider = [](neon::Application* app,
                                                  const neon::ModelCreateInfo& info,
                                                  const neon::Model* model) {
            std::vector indices = {
                neon::StorageBufferInstanceData::Slot(
                    sizeof(GPUNeuronSegment),
                    sizeof(GPUNeuronSegment),
                    0,
                    model->getUniformBuffer()
                )
            };
            return std::vector<neon::InstanceData*>{new neon::StorageBufferInstanceData(app, info, indices)};
        };

        _neuronModel = std::make_shared<neon::Model>(app, "Neuron", modelCreateInfo);
        render->getRoom()->markUsingModel(_neuronModel.get());

        _selector = NeuronSelector(this, _neuronModel->getUniformBuffer(), 1);
    }

    NeuronScene::~NeuronScene() {
        if (_neuronModel != nullptr) {
            _render->getRoom()->unmarkUsingModel(_neuronModel.get());
        }
    }

    const std::vector<Neuron>& NeuronScene::getNeurons() const {
        return _neurons;
    }

    AbstractSelector& NeuronScene::getSelector() {
        return _selector;
    }

    const AbstractSelector& NeuronScene::getSelector() const {
        return _selector;
    }

    size_t NeuronScene::getNeuronsAmount() {
        return _neurons.size();
    }

    std::optional<Neuron*> NeuronScene::findNeuron(UID uid) {
        auto it = std::find_if(
            _neurons.begin(), _neurons.end(),
            [uid](const Neuron& neuron) {
                return uid == neuron.getId();
            }
        );
        if (it == _neurons.end()) return {};
        return {&*it};
    }

    std::optional<const Neuron*> NeuronScene::findNeuron(UID uid) const {
        auto it = std::find_if(
            _neurons.cbegin(), _neurons.cend(),
            [uid](const Neuron& neuron) {
                return uid == neuron.getId();
            }
        );
        if (it == _neurons.cend()) return {};
        return {&*it};
    }

    std::optional<GPUNeuron*> NeuronScene::findGPUNeuron(UID uid) {
        auto it = std::find_if(
            _neurons.cbegin(), _neurons.cend(),
            [uid](const Neuron& neuron) {
                return uid == neuron.getId();
            }
        );
        if (it == _neurons.cend()) return {};
        ptrdiff_t index = it - _neurons.begin();

        return &_gpuNeurons[index];
    }

    std::optional<const GPUNeuron*> NeuronScene::findGPUNeuron(UID uid) const {
        auto it = std::find_if(
            _neurons.cbegin(), _neurons.cend(),
            [uid](const Neuron& neuron) {
                return uid == neuron.getId();
            }
        );
        if (it == _neurons.cend()) return {};
        ptrdiff_t index = it - _neurons.begin();

        return &_gpuNeurons[index];
    }

    bool NeuronScene::addNeuron(const PrototypeNeuron& neuron) {
        auto result = Neuron::fromPrototype(neuron);
        if (result.isOk()) {
            addNeuron(std::move(result.getResult()));
            return true;
        }

        neon::Logger::defaultLogger()->error(result.getError());
        return false;
    }

    void NeuronScene::addNeuron(const Neuron& neuron) {
        _neurons.push_back(neuron);
        _gpuNeurons.emplace_back(_neuronModel, 0, &_neurons.back());

        if (_neurons.size() == 1) {
            _sceneBoundingBox = neuron.getBoundingBox();
        } else {
            combineBoundingBoxes(neuron.getBoundingBox());
        }
    }

    void NeuronScene::addNeuron(Neuron&& neuron) {
        auto bb = neuron.getBoundingBox();
        _neurons.push_back(std::move(neuron));
        _gpuNeurons.emplace_back(_neuronModel, 0, &_neurons.back());
        if (_neurons.size() == 1) {
            _sceneBoundingBox = bb;
        } else {
            combineBoundingBoxes(bb);
        }
    }

    bool NeuronScene::removeNeuron(UID neuronId) {
        auto it = std::find_if(
            _neurons.begin(),
            _neurons.end(),
            [&neuronId](const Neuron& neuron) {
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

    rush::AABB<3, float> NeuronScene::getSceneBoundingBox() const {
        return _sceneBoundingBox;
    }
}
