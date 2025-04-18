//
// Created by gaeqs on 8/10/24.
//

#include "NeoneuronRender.h"

#include "synapse/SynapseRepresentation.h"

#include <vector>
#include <neon/util/DeferredUtils.h>

#include <neoneuron/render/component/GlobalParametersUpdaterComponent.h>
#include <neoneuron/render/component/camera/OrbitalCameraController.h>

#include <neoneuron/render/complex/ComplexNeuronRepresentation.h>

CMRC_DECLARE(resources);

namespace neoneuron
{
    NeoneuronRender::Components::Components(NeoneuronRender* render) :
        ui(render),
        cameraData(render)
    {
    }

    std::shared_ptr<neon::Render> NeoneuronRender::initRender()
    {
        neon::AssetLoaderContext context(&_application, nullptr, &_fileSystem);
        auto render = neon::loadAssetFromFile<neon::Render>("render.json", context);
        if (render == nullptr) {
            neon::error() << "Render is null!";
        }
        auto fb = _application.getAssets().get<neon::FrameBuffer>("neoneuron:frame_buffer").value_or(nullptr);
        _renderFrameBuffer = std::dynamic_pointer_cast<neon::SimpleFrameBuffer>(fb);
        _renderFrameBuffer->setClearColor(0, rush::Vec4f(0.0f, 0.0f, 0.0f, 0.0f));

        auto outFb = _application.getAssets().get<neon::FrameBuffer>("neoneuron:picker_frame_buffer").value_or(nullptr);
        _outFrameBuffer = std::dynamic_pointer_cast<neon::SimpleFrameBuffer>(outFb);
        return render;
    }

    void NeoneuronRender::initGameObjects()
    {
        auto parameterUpdaterGO = _room->newGameObject();
        parameterUpdaterGO->setName("Parameter updater");
        parameterUpdaterGO->newComponent<GlobalParametersUpdaterComponent>(*this);
    }

    void NeoneuronRender::initSelectionResolver()
    {
        neon::AssetLoaderContext context(&_application, nullptr, &_fileSystem);
        _selectionResolver = neon::loadAssetFromFile<neon::Model>("model/selector/selector_resolver.json", context);
        _selectionResolver->getInstanceData(0)->createInstance();
        _room->markUsingModel(_selectionResolver.get());

        auto colorOutput = _renderFrameBuffer->getOutputs()[0].resolvedTexture;
        auto selectionOutput = _renderFrameBuffer->getOutputs()[1].texture;
        auto& materials = _selectionResolver->getMeshes()[0]->getMaterials();
        for (auto& material : materials) {
            auto& ubo = material->getUniformBuffer();
            ubo->setTexture(0, colorOutput);
            ubo->setTexture(1, selectionOutput);
            ubo->uploadData(3, 0);
        }
    }

    NeoneuronRender::NeoneuronRender(NeoneuronApplication* neoneuron,
                                     const neon::vulkan::VKApplicationCreateInfo& createInfo) :
        _neoneuronApplication(neoneuron),
        _application(std::make_unique<neon::vulkan::VKApplication>(createInfo)),
        _fileSystem(cmrc::resources::get_filesystem())
    {
        _application.init();
        _application.setRender(initRender());

        _room = std::make_shared<neon::Room>(&_application);
        _room->getCamera().setFrustum(_room->getCamera().getFrustum().withFar(10000.0f));
        _application.setRoom(_room);
        _startTime = std::chrono::high_resolution_clock::now();

        _components = std::make_unique<Components>(this);

        initGameObjects();
        initSelectionResolver();

        _representations.push_back(std::make_unique<ComplexNeuronRepresentation>(this));
        //_representations.push_back(std::make_unique<SynapseRepresentation>(this));
    }

    NeoneuronRender::~NeoneuronRender()
    {
        _room->unmarkUsingModel(_selectionResolver.get());
    }

    NeoneuronApplication* NeoneuronRender::getNeoneuronApplication()
    {
        return _neoneuronApplication;
    }

    const NeoneuronApplication* NeoneuronRender::getNeoneuronApplication() const
    {
        return _neoneuronApplication;
    }

    neon::Application& NeoneuronRender::getApplication()
    {
        return _application;
    }

    const neon::Application& NeoneuronRender::getApplication() const
    {
        return _application;
    }

    const neon::CMRCFileSystem NeoneuronRender::getFileSystem() const
    {
        return _fileSystem;
    }

    const std::shared_ptr<neon::SimpleFrameBuffer>& NeoneuronRender::getRenderFrameBuffer() const
    {
        return _renderFrameBuffer;
    }

    const std::shared_ptr<neon::SimpleFrameBuffer>& NeoneuronRender::getOutFrameBuffer() const
    {
        return _outFrameBuffer;
    }

    const std::shared_ptr<neon::Room>& NeoneuronRender::getRoom() const
    {
        return _room;
    }

    NeoneuronUI& NeoneuronRender::getUI()
    {
        return _components->ui;
    }

    const NeoneuronUI& NeoneuronRender::getUI() const
    {
        return _components->ui;
    }

    CameraData& NeoneuronRender::getCameraData()
    {
        return _components->cameraData;
    }

    const CameraData& NeoneuronRender::getCameraData() const
    {
        return _components->cameraData;
    }

    NeoneuronRenderData& NeoneuronRender::getRenderData()
    {
        return _renderData;
    }

    const NeoneuronRenderData& NeoneuronRender::getRenderData() const
    {
        return _renderData;
    }

    std::vector<AbstractNeuronRepresentation*> NeoneuronRender::getRepresentations()
    {
        std::vector<AbstractNeuronRepresentation*> result;
        for (auto& rep : _representations) {
            result.push_back(rep.get());
        }

        return result;
    }

    std::vector<const AbstractNeuronRepresentation*> NeoneuronRender::getRepresentations() const
    {
        std::vector<const AbstractNeuronRepresentation*> result;
        for (auto& rep : _representations) {
            result.push_back(rep.get());
        }

        return result;
    }

    void NeoneuronRender::setSkybox(const std::shared_ptr<neon::Texture>& skybox) const
    {
        auto& materials = _selectionResolver->getMeshes()[0]->getMaterials();
        for (auto& material : materials) {
            auto& ubo = material->getUniformBuffer();
            ubo->setTexture(2, skybox);
            ubo->uploadData(3, 1);
        }
    }

    rush::AABB<3, float> NeoneuronRender::getCombinedAABB() const
    {
        if (_representations.empty()) {
            return {};
        }
        rush::AABB<3, float> aabb = _representations[0]->getSceneBoundingBox();

        for (size_t i = 1; i < _representations.size(); i++) {
            auto repAABB = _representations[1]->getSceneBoundingBox();
            auto min = rush::min(aabb.center - aabb.radius, repAABB.center - repAABB.radius);
            auto max = rush::max(aabb.center + aabb.radius, repAABB.center + repAABB.radius);
            aabb = rush::AABB<3, float>::fromEdges(min, max);
        }

        return aabb;
    }

    float NeoneuronRender::getCurrentTime() const
    {
        auto now = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::duration<float>>(now - _startTime).count();
    }

    bool NeoneuronRender::renderLoop()
    {
        auto result = _application.startGameLoop();
        if (!result.isOk()) {
            neon::error() << result.getError();
        }
        return result.isOk();
    }

    void NeoneuronRender::focusScene() const
    {
        _components->cameraData.getCameraController()->focusOn(getCombinedAABB());
    }

    void NeoneuronRender::refreshNeuronProperty(GID neuronId, const std::string& propertyName)
    {
        for (auto& rep : _representations) {
            rep->refreshNeuronProperty(neuronId, propertyName);
        }
    }

    void NeoneuronRender::removeRepresentation(AbstractNeuronRepresentation* representation)
    {
        if (representation == nullptr) {
            return;
        }

        auto it = std::ranges::find_if(_representations, [&](const std::unique_ptr<AbstractNeuronRepresentation>& it) {
            return it.get() == representation;
        });
        if (it == _representations.end()) {
            return;
        }

        _representations.erase(it);
    }
} // namespace neoneuron
