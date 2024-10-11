//
// Created by gaeqs on 9/10/24.
//

#include "NeuronScene.h"

#include <neoneuron/render/NeoneuronRender.h>

CMRC_DECLARE(resources);

neoneuron::NeuronScene::NeuronScene(NeuronScene&& other) noexcept
    : _neuronModel(std::move(other._neuronModel)),
      _neurons(std::move(other._neurons)),
      _gpuNeurons(std::move(other._gpuNeurons)) {}

neoneuron::NeuronScene& neoneuron::NeuronScene::operator=(NeuronScene&& other) noexcept {
    if (this == &other) return *this;
    _neuronModel = std::move(other._neuronModel);
    _neurons = std::move(other._neurons);
    _gpuNeurons = std::move(other._gpuNeurons);
    return *this;
}

neoneuron::NeuronScene::NeuronScene(NeoneuronRender* render) : _render(render) {
    auto* app = &render->getApplication();

    auto shader = std::make_shared<neon::ShaderProgram>(&render->getApplication(), "Neuron");

    auto fs = cmrc::resources::get_filesystem();
    shader->addShader(neon::ShaderType::TASK, fs.open("neuron.task"));
    shader->addShader(neon::ShaderType::MESH, fs.open("neuron.mesh"));
    shader->addShader(neon::ShaderType::FRAGMENT, fs.open("neuron.frag"));

    if (auto result = shader->compile(); result.has_value()) {
        render->getApplication().getLogger().error(result.value());
        return;
    }

    std::vector modelBindings = {neon::ShaderUniformBinding::storageBuffer(sizeof(GPUNeuronSegment) * 1000000)};
    auto modelDescriptor = std::make_shared<neon::ShaderUniformDescriptor>(app, "model", modelBindings);

    neon::MaterialCreateInfo materialCreateInfo(render->getRenderFrameBuffer(), shader);
    materialCreateInfo.descriptions.extraUniforms.push_back(modelDescriptor);
    auto material = std::make_shared<neon::Material>(app, "Neuron", materialCreateInfo);

    auto drawable = std::make_shared<neon::MeshShaderDrawable>(app, "Neuron", material);
    drawable->setGroupsSupplier([](const neon::Model& model) {
        return rush::Vec<3, uint32_t>{model.getInstanceData(0)->getInstanceAmount(), 1, 1};
    });

    neon::ModelCreateInfo modelCreateInfo;
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
}

neoneuron::NeuronScene::~NeuronScene() {
    if (_neuronModel != nullptr) {
        _render->getRoom()->unmarkUsingModel(_neuronModel.get());
    }
}

void neoneuron::NeuronScene::addNeuron(const Neuron& neuron) {
    _neurons.push_back(neuron);
    _gpuNeurons.emplace_back(_neuronModel->getInstanceData(0), &_neurons.back());
}

void neoneuron::NeuronScene::addNeuron(Neuron&& neuron) {
    _neurons.push_back(std::move(neuron));
    _gpuNeurons.emplace_back(_neuronModel->getInstanceData(0), &_neurons.back());
}

void neoneuron::NeuronScene::removeNeuron(UID neuronId) {
    auto it = std::find_if(
        _neurons.begin(),
        _neurons.end(),
        [&neuronId](const Neuron& neuron) {
            return neuron.getId() == neuronId;
        }
    );
    if (it == _neurons.end()) return;
    ptrdiff_t index = it - _neurons.begin();
    _neurons.erase(_neurons.begin() + index);
    _gpuNeurons.erase(_gpuNeurons.begin() + index);
    for (auto& gpuNeuron: _gpuNeurons) {
        gpuNeuron.refreshGPUData();
    }
}
