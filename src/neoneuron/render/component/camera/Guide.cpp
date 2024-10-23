//
// Created by gaeqs on 23/10/2024.
//

#include "Guide.h"

#include <neoneuron/render/NeoneuronRender.h>

CMRC_DECLARE(resources);

namespace neoneuron {
    void Guide::updateState(bool active) {
        _model->getInstanceData(0)->uploadData(
            _instance, 0, GuideInstancingData{
                active ? 1.0f : 0.0f,
                _render->getCurrentTime()
            }
        );
    }

    Guide::Guide(NeoneuronRender* render)
        : _render(render),
          _positionListener([this](bool active) {
              updateState(active);
          }) {}

    Guide::~Guide() {
        if (_model != nullptr) {
            getRoom()->unmarkUsingModel(_model.get());
        }
    }

    void Guide::onStart() {
        auto* app = getApplication();

        auto shader = std::make_shared<neon::ShaderProgram>(app, "Neuron");

        auto fs = cmrc::resources::get_filesystem();
        shader->addShader(neon::ShaderType::VERTEX, fs.open("shader/guide/guide.vert"));
        shader->addShader(neon::ShaderType::FRAGMENT, fs.open("shader/guide/guide.frag"));

        if (auto result = shader->compile(); result.has_value()) {
            app->getLogger().error(result.value());
            return;
        }

        neon::InputDescription vertexAttribute(sizeof(rush::Vec3f), neon::InputRate::VERTEX);
        vertexAttribute.addAttribute(3, 0);

        neon::MaterialCreateInfo materialCreateInfo(_render->getRenderFrameBuffer(), shader);


        materialCreateInfo.rasterizer.cullMode = neon::CullMode::NONE;
        materialCreateInfo.descriptions.vertex.push_back(vertexAttribute);
        materialCreateInfo.descriptions.instance.push_back(GuideInstancingData::getInstancingDescription());

        neon::MaterialAttachmentBlending blending;
        blending.blend = true;
        blending.alphaBlendOperation = neon::BlendOperation::ADD;
        blending.alphaSourceBlendFactor = neon::BlendFactor::ONE;
        blending.alphaDestinyBlendFactor = neon::BlendFactor::ONE;

        materialCreateInfo.blending.attachmentsBlending.push_back(blending);

        auto material = std::make_shared<neon::Material>(app, "Guide", materialCreateInfo);
        material->setPriority(-1);

        auto mesh = std::make_shared<neon::Mesh>(app, "Neuron", material);

        std::vector<rush::Vec3f> vertices = {
            rush::Vec3f(-100000.0f, 0.0f, -100000.0f),
            rush::Vec3f(100000.0f, 0.0f, -100000.0f),
            rush::Vec3f(-100000.0f, 0.0f, 100000.0f),
            rush::Vec3f(100000.0f, 0.0f, 100000.0f),
        };

        std::vector<uint32_t> indices = {0, 1, 2, 1, 2, 3};

        mesh->uploadVertices(vertices);
        mesh->uploadIndices(indices);

        neon::ModelCreateInfo modelCreateInfo;
        modelCreateInfo.maximumInstances = 1;
        modelCreateInfo.drawables.push_back(mesh);
        modelCreateInfo.defineInstanceType<GuideInstancingData>();

        _model = std::make_shared<neon::Model>(app, "Neuron", modelCreateInfo);
        getRoom()->markUsingModel(_model.get());

        _instance = _model->getInstanceData(0)->createInstance().getResult();

        // Init Hey!
        _render->getCameraData().onActivePosition() += _positionListener;

        updateState(false);
    }
}
