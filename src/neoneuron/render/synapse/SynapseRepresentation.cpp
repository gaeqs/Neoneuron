//
// Created by gaeqs on 31/03/25.
//

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

    void SynapseRepresentation::loadMaterial()
    {
        auto* app = &_render->getApplication();
        neon::MaterialCreateInfo materialCreateInfo(_render->getRenderFrameBuffer(), _shader);
        materialCreateInfo.rasterizer.cullMode = neon::CullMode::BACK;
        materialCreateInfo.rasterizer.polygonMode = neon::PolygonMode::FILL;
        materialCreateInfo.descriptions.uniformBindings[UNIFORM_SET] = neon::DescriptorBinding::extra(_uboDescriptor);
        _material = std::make_shared<neon::Material>(app, "neoneuron:synapse", materialCreateInfo);
    }

    void SynapseRepresentation::loadModel()
    {
        auto* app = &_render->getApplication();

        auto drawable = std::make_shared<neon::MeshShaderDrawable>(app, "neoneuron:synapse", _material);
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

    void SynapseRepresentation::onSynapseAdded(mindset::Synapse* synapse)
    {
        auto [it, ok] = _gpuSynapses.emplace(std::piecewise_construct, std::forward_as_tuple(synapse->getUID()),
                                             std::forward_as_tuple(_createInfo, synapse));
        if (ok) {
            if (_gpuSynapses.size() == 1) {
                recalculateBoundingBox();
            } else {
                addSynapseToBoundingBox(it->second);
            }
            updateGPURepresentationData();
        }
    }

    void SynapseRepresentation::onSynapseRemoved(mindset::UID uid)
    {
        _gpuSynapses.erase(uid);
        recalculateBoundingBox();
        updateGPURepresentationData();
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
        loadMaterial();
        loadModel();

        auto& dataset = render->getNeoneuronApplication()->getDataset();

        _createInfo =
            GPUSynapseCreateInfo(&dataset, _model->getInstanceData(0),
                                 dataset.getProperties().defineProperty(mindset::PROPERTY_SYNAPSE_PRE_POSITION),
                                 dataset.getProperties().defineProperty(mindset::PROPERTY_SYNAPSE_POST_POSITION));

        _synapseAddListener = [this](mindset::Synapse* synapse) { onSynapseAdded(synapse); };
        _synapseRemoveListener = [this](mindset::UID uid) { onSynapseRemoved(uid); };
        _clearListener = [this](void*) { onClear(); };

        auto& circuit = render->getNeoneuronApplication()->getDataset().getCircuit();
        circuit.getSynapseAddedEvent() += _synapseAddListener;
        circuit.getSynapseRemovedEvent() += _synapseRemoveListener;
        circuit.getClearEvent() += _clearListener;
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

    void SynapseRepresentation::refreshNeuronProperty(mindset::UID neuronId, const std::string& propertyName)
    {
    }
} // namespace neoneuron
