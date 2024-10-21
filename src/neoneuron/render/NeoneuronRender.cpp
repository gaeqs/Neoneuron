//
// Created by gaeqs on 8/10/24.
//

#include "NeoneuronRender.h"

#include <vector>

#include <neoneuron/render/component/GlobalParametersUpdaterComponent.h>
#include <neoneuron/render/component/camera/InstantCameraInterpolator.h>
#include <neoneuron/render/component/camera/OrbitalCameraController.h>


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
            true
        );

        auto scFramebuffer = std::make_shared<neon::SwapChainFrameBuffer>(&_application, false);

        render->addRenderPass(std::make_shared<neon::DefaultRenderPassStrategy>(_renderFrameBuffer));
        render->addRenderPass(std::make_shared<neon::DefaultRenderPassStrategy>(scFramebuffer));

        return render;
    }

    void NeoneuronRender::initGameObjects() {
        auto parameterUpdaterGO = _room->newGameObject();
        parameterUpdaterGO->setName("Parameter updater");
        parameterUpdaterGO->newComponent<GlobalParametersUpdaterComponent>();

        auto cameraGO = _room->newGameObject();
        cameraGO->setName("Camera controller");

        _cameraController = cameraGO->newComponent<OrbitalCameraController>(
            std::make_unique<InstantCameraInterpolator>(&_room->getCamera())
        );
    }

    NeoneuronRender::NeoneuronRender(const neon::vulkan::VKApplicationCreateInfo& createInfo)
        : _application(std::make_unique<neon::vulkan::VKApplication>(createInfo)) {
        _application.init();
        _application.setRender(initRender());

        _room = std::make_shared<neon::Room>(&_application);
        _room->getCamera().setFrustum(_room->getCamera().getFrustum().withFar(10000.0f));
        _application.setRoom(_room);
        _neuronScene = NeuronScene(this);
        _ui = NeoneuronUI(*this);

        initGameObjects();
    }

    NeoneuronRender::~NeoneuronRender() {
        _neuronScene = NeuronScene();
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

    neon::IdentifiableWrapper<CameraController> NeoneuronRender::getCameraController() const {
        return _cameraController;
    }

    bool NeoneuronRender::renderLoop() {
        return _application.startGameLoop().isOk();
    }

    void NeoneuronRender::focusScene() const {
        _cameraController->focusOn(_neuronScene.getSceneBoundingBox());
    }
}
