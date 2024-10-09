//
// Created by gaeqs on 8/10/24.
//

#include "NeoneuronRender.h"

#include <vector>

namespace neoneuron {
    std::shared_ptr<neon::Render> NeoneuronRender::initRender() {
        std::vector<neon::ShaderUniformBinding> bindings = {};

        auto descriptor = std::make_shared<neon::ShaderUniformDescriptor>(&_application, "Descriptor", bindings);
        auto render = std::make_shared<neon::Render>(&_application, "Render", descriptor);

        auto frameBuffer = std::make_shared<neon::SimpleFrameBuffer>(
            &_application,
            std::vector<neon::FrameBufferTextureCreateInfo>{
                neon::TextureFormat::R8G8B8A8,
            },
            false
        );

        auto scFramebuffer = std::make_shared<neon::SwapChainFrameBuffer>(&_application, false);

        render->addRenderPass(std::make_shared<neon::DefaultRenderPassStrategy>(frameBuffer));
        render->addRenderPass(std::make_shared<neon::DefaultRenderPassStrategy>(scFramebuffer));

        return render;
    }

    NeoneuronRender::NeoneuronRender(const neon::vulkan::VKApplicationCreateInfo& createInfo)
        : _application(std::make_unique<neon::vulkan::VKApplication>(createInfo)) {
        _application.init();
        _application.setRender(initRender());

        _room = std::make_shared<neon::Room>(&_application);
        _application.setRoom(_room);

        _ui = std::make_unique<NeoneuronUI>(_room.get());
    }

    neon::Application& NeoneuronRender::getApplication() {
        return _application;
    }

    const neon::Application& NeoneuronRender::getApplication() const {
        return _application;
    }

    bool NeoneuronRender::renderLoop() {
        return _application.startGameLoop().isOk();
    }
}
