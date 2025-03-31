//
// Created by gaeqs on 8/10/24.
//

#ifndef NEONEURONRENDER_H
#define NEONEURONRENDER_H

#include <neon/Neon.h>

#include <neoneuron/ui/NeoneuronUI.h>
#include <neoneuron/render/component/camera/CameraData.h>
#include <neoneuron/render/AbstractNeuronRepresentation.h>
#include <neoneuron/render/NeoneuronRenderData.h>

namespace neoneuron
{
    class NeoneuronApplication;

    class NeoneuronRender
    {
        struct Components
        {
            NeoneuronUI ui;
            CameraData cameraData;

            Components(NeoneuronRender* render);
        };

        NeoneuronApplication* _neoneuronApplication;
        neon::Application _application;
        neon::CMRCFileSystem _fileSystem;
        std::shared_ptr<neon::SimpleFrameBuffer> _renderFrameBuffer;
        std::shared_ptr<neon::Room> _room;
        std::chrono::high_resolution_clock::time_point _startTime;
        std::unique_ptr<Components> _components;
        NeoneuronRenderData _renderData;
        std::shared_ptr<neon::Model> _selectionResolver;

        std::vector<std::unique_ptr<AbstractNeuronRepresentation>> _representations;

        std::shared_ptr<neon::Render> initRender();

        void initGameObjects();

        void initSelectionResolver();

      public:
        NeoneuronRender(const NeoneuronRender& other) = delete;

        explicit NeoneuronRender(NeoneuronApplication* neoneuron,
                                 const neon::vulkan::VKApplicationCreateInfo& createInfo);

        ~NeoneuronRender();

        [[nodiscard]] NeoneuronApplication* getNeoneuronApplication();

        [[nodiscard]] const NeoneuronApplication* getNeoneuronApplication() const;

        [[nodiscard]] neon::Application& getApplication();

        [[nodiscard]] const neon::Application& getApplication() const;

        const neon::CMRCFileSystem getFileSystem() const;

        [[nodiscard]] const std::shared_ptr<neon::SimpleFrameBuffer>& getRenderFrameBuffer() const;

        [[nodiscard]] const std::shared_ptr<neon::Room>& getRoom() const;

        [[nodiscard]] NeoneuronUI& getUI();

        [[nodiscard]] const NeoneuronUI& getUI() const;

        [[nodiscard]] CameraData& getCameraData();

        [[nodiscard]] const CameraData& getCameraData() const;

        [[nodiscard]] NeoneuronRenderData& getRenderData();

        [[nodiscard]] const NeoneuronRenderData& getRenderData() const;

        [[nodiscard]] std::vector<AbstractNeuronRepresentation*> getRepresentations();

        [[nodiscard]] std::vector<const AbstractNeuronRepresentation*> getRepresentations() const;

        void setSkybox(const std::shared_ptr<neon::Texture>& skybox) const;

        rush::AABB<3, float> getCombinedAABB() const;

        float getCurrentTime() const;

        bool renderLoop();

        void focusScene() const;

        void refreshNeuronProperty(mindset::UID neuronId, const std::string& propertyName);

    };
} // namespace neoneuron

#endif //NEONEURONRENDER_H
