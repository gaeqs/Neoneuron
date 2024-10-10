//
// Created by gaeqs on 8/10/24.
//

#include "NeoneuronRender.h"

#include <neoneuron/render/component/GlobalParametersUpdaterComponent.h>
#include <neoneuron/render/component/LockMouseComponent.h>

#include <vector>
#include <neon/util/component/CameraMovementComponent.h>



namespace neoneuron {
    std::shared_ptr<neon::Render> NeoneuronRender::initRender() {
        std::vector<neon::ShaderUniformBinding> bindings = {
            {neon::UniformBindingType::UNIFORM_BUFFER, sizeof(Matrices)},
        };

        auto descriptor = std::make_shared<neon::ShaderUniformDescriptor>(&_application, "Descriptor", bindings);
        auto render = std::make_shared<neon::Render>(&_application, "Render", descriptor);

        _renderFrameBuffer = std::make_shared<neon::SimpleFrameBuffer>(
            &_application,
            std::vector<neon::FrameBufferTextureCreateInfo>{
                neon::TextureFormat::R8G8B8A8,
            },
            false
        );

        auto scFramebuffer = std::make_shared<neon::SwapChainFrameBuffer>(&_application, false);

        render->addRenderPass(std::make_shared<neon::DefaultRenderPassStrategy>(_renderFrameBuffer));
        render->addRenderPass(std::make_shared<neon::DefaultRenderPassStrategy>(scFramebuffer));

        return render;
    }

    NeoneuronRender::NeoneuronRender(const neon::vulkan::VKApplicationCreateInfo& createInfo)
        : _application(std::make_unique<neon::vulkan::VKApplication>(createInfo)) {
        _application.init();
        _application.setRender(initRender());

        _room = std::make_shared<neon::Room>(&_application);
        _application.setRoom(_room);
        _ui = NeoneuronUI(_room.get());
        _neuronScene = NeuronScene(this);

        auto parameterUpdater = _room->newGameObject();
        parameterUpdater->newComponent<GlobalParametersUpdaterComponent>();
        auto movement =
            parameterUpdater->newComponent<neon::CameraMovementComponent>();
        movement->setSpeed(10.0f);
        parameterUpdater->newComponent<LockMouseComponent>(movement);
    }

    neon::Application& NeoneuronRender::getApplication() {
        return _application;
    }

    const neon::Application& NeoneuronRender::getApplication() const {
        return _application;
    }

    const std::shared_ptr<neon::FrameBuffer>& NeoneuronRender::getRenderFrameBuffer() const {
        return _renderFrameBuffer;
    }

    const std::shared_ptr<neon::Room>& NeoneuronRender::getRoom() const {
        return _room;
    }

    NeuronScene& NeoneuronRender::getNeuronScene() {
        return _neuronScene;
    }

    const NeuronScene& NeoneuronRender::getNeuronScene() const {
        return _neuronScene;
    }

    bool NeoneuronRender::renderLoop() {
        return _application.startGameLoop().isOk();
    }
}
