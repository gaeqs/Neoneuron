//
// Created by gaeqs on 18/04/2025.
//

#include "Viewport.h"

namespace neoneuron
{

    void Viewport::loadSelectionResolver()
    {
        // This asset collection is used to provide local information to the asset loader.
        // It has a higher priority than the application's asset collection!
        neon::AssetCollection localCollection;
        localCollection.store(_outputFrameBuffer, neon::AssetStorageMode::WEAK);

        auto* application = &_render->getApplication();
        auto fileSystem = _render->getFileSystem();
        neon::AssetLoaderContext context(application, nullptr, &fileSystem);
        context.localCollection = &localCollection;

        _selectionResolver = neon::loadAssetFromFile<neon::Model>("model/selector/selector_resolver.json", context);

        // TODO I think this is not needed?
        _selectionResolver->getInstanceData(0)->createInstance();

        _render->getRoom()->markUsingModel(_selectionResolver.get());

        auto colorOutput = _inputFrameBuffer->getOutputs()[0].resolvedTexture;
        auto selectionOutput = _inputFrameBuffer->getOutputs()[1].texture;
        auto& materials = _selectionResolver->getMeshes()[0]->getMaterials();
        for (auto& material : materials) {
            auto& ubo = material->getUniformBuffer();
            ubo->setTexture(0, colorOutput);
            ubo->setTexture(1, selectionOutput);
            ubo->uploadData(3, 0);
        }
    }

    void Viewport::loadViewportComponent()
    {
        auto gameObject = _render->getRoom()->newGameObject();
        _viewportComponent = gameObject->newComponent<neon::ViewportComponent>(_outputFrameBuffer);
    }

    Viewport::Viewport(NeoneuronRender* render, int priority) :
        _render(render)
    {
        auto* application = &render->getApplication();
        auto fileSystem = render->getFileSystem();
        auto neonRender = application->getRender();

        // Two framebuffer:
        // - One used by representations to draw its elements. 8 samples. Three textures:
        //   - neoneuron:color: R8G8B8A8. Used to draw the color. Resolved to a texture with 1 sample.
        //     - Resolved texture's name: neoneuron:resolved_color.
        //   - neoneuron:picker: R32FG32FB32FA32F. Used to store the information of the drawn element.
        //   - neoneuron:depth: D32F. Used to store the depth of the drawn elements.
        // - One used to process the data of the first framebuffer.
        //   - Transforms the picker texture into a texture with 1 sample.
        //   - Draws the background of the scene.
        //   - Same textures than the first framebuffer, but with only 1 sample and without depth.
        std::vector<neon::FrameBufferTextureCreateInfo> textures = {neon::TextureFormat::R8G8B8A8,
                                                                    neon::TextureFormat::R32FG32FB32FA32F};

        textures[0].name = "neoneuron:color";
        textures[0].resolveName = "neoneuron:resolved_color";
        textures[1].name = "neoneuron:picker";

        _inputFrameBuffer = std::make_shared<neon::SimpleFrameBuffer>(
            application, "neoneuron:input", textures, true, "neoneuron:depth", neon::SamplesPerTexel::COUNT_8);
        _outputFrameBuffer = std::make_shared<neon::SimpleFrameBuffer>(application, "neoneuron:output", textures, false,
                                                                       "neoneuron:depth");

        _inputStrategy =
            std::make_shared<neon::DefaultRenderPassStrategy>("neoneuron:input", _inputFrameBuffer, priority);

        _outputStrategy =
            std::make_shared<neon::DefaultRenderPassStrategy>("neoneuron:input", _outputFrameBuffer, priority);

        neonRender->addRenderPass(_inputStrategy);
        neonRender->addRenderPass(_outputStrategy);

        loadSelectionResolver();
        loadViewportComponent();
    }

    Viewport::~Viewport()
    {
        _render->getRoom()->unmarkUsingModel(_selectionResolver.get());
        _viewportComponent->getGameObject()->destroy();

        auto neonRender = _render->getApplication().getRender();
        neonRender->removeRenderPass(_inputStrategy);
        neonRender->removeRenderPass(_outputStrategy);
    }

    std::shared_ptr<neon::SimpleFrameBuffer> Viewport::getInputFrameBuffer() const
    {
        return _inputFrameBuffer;
    }

    neon::IdentifiableWrapper<neon::ViewportComponent> Viewport::getViewportComponent() const
    {
        return _viewportComponent;
    }

    void Viewport::setSkybox(const std::shared_ptr<neon::Texture>& skybox) const
    {
        auto& materials = _selectionResolver->getMeshes()[0]->getMaterials();
        if (skybox == nullptr) {
            for (auto& material : materials) {
                auto& ubo = material->getUniformBuffer();
                ubo->uploadData(3, 0);
            }
        } else {
            for (auto& material : materials) {
                auto& ubo = material->getUniformBuffer();
                ubo->setTexture(2, skybox);
                ubo->uploadData(3, 1);
            }
        }
    }
} // namespace neoneuron