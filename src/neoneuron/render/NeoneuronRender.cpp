//
// Created by gaeqs on 8/10/24.
//

#include "NeoneuronRender.h"

#include <vector>

#include <neoneuron/render/component/GlobalParametersUpdaterComponent.h>
#include <neoneuron/render/component/camera/OrbitalCameraController.h>


CMRC_DECLARE(resources);

namespace neoneuron {
    NeoneuronRender::Components::Components(NeoneuronRender* render) :
        neuronScene(render),
        ui(render),
        cameraData(render) {}

    std::shared_ptr<neon::Render> NeoneuronRender::initRender() {
        neon::AssetLoaderContext context(&_application, nullptr, &_fileSystem);
        auto render = neon::loadAssetFromFile<neon::Render>("render.json", context);
        if (render == nullptr) {
            _application.getLogger().error("Render is null!");
        }
        _renderFrameBuffer = _application.getAssets()
                .get<neon::FrameBuffer>("neoneuron:frame_buffer")
                .value_or(nullptr);
        return render;
    }

    void NeoneuronRender::initGameObjects() {
        auto parameterUpdaterGO = _room->newGameObject();
        parameterUpdaterGO->setName("Parameter updater");
        parameterUpdaterGO->newComponent<GlobalParametersUpdaterComponent>(*this);
    }

    NeoneuronRender::NeoneuronRender(const neon::vulkan::VKApplicationCreateInfo& createInfo)
        : _application(std::make_unique<neon::vulkan::VKApplication>(createInfo)),
          _fileSystem(cmrc::resources::get_filesystem()) {
        _application.init();
        _application.setRender(initRender());

        _room = std::make_shared<neon::Room>(&_application);
        _room->getCamera().setFrustum(_room->getCamera().getFrustum().withFar(10000.0f));
        _application.setRoom(_room);
        _startTime = std::chrono::high_resolution_clock::now();

        _components = std::make_unique<Components>(this);

        initGameObjects();
    }

    NeoneuronRender::~NeoneuronRender() = default;

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
        return _components->neuronScene;
    }

    const NeuronScene& NeoneuronRender::getNeuronScene() const {
        return _components->neuronScene;
    }

    CameraData& NeoneuronRender::getCameraData() {
        return _components->cameraData;
    }

    const CameraData& NeoneuronRender::getCameraData() const {
        return _components->cameraData;
    }

    float NeoneuronRender::getCurrentTime() const {
        auto now = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::duration<float>>(now - _startTime).count();
    }

    bool NeoneuronRender::renderLoop() {
        auto result = _application.startGameLoop();
        if (!result.isOk()) {
            neon::Logger::defaultLogger()->error(result.getError());
        }
        return result.isOk();
    }

    void NeoneuronRender::focusScene() const {
        _components->cameraData.getCameraController()->focusOn(_components->neuronScene.getSceneBoundingBox());
    }
}
