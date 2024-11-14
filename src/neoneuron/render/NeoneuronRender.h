//
// Created by gaeqs on 8/10/24.
//

#ifndef NEONEURONRENDER_H
#define NEONEURONRENDER_H

#include <neon/Neon.h>

#include <neoneuron/ui/NeoneuronUI.h>
#include <neoneuron/render/NeuronScene.h>
#include <neoneuron/render/component/camera/CameraData.h>


namespace neoneuron {
    class NeoneuronRender {
        struct Components {
            NeuronScene neuronScene;
            NeoneuronUI ui;
            CameraData cameraData;

            Components(NeoneuronRender* render);
        };

        neon::Application _application;
        neon::CMRCFileSystem _fileSystem;
        std::shared_ptr<neon::FrameBuffer> _renderFrameBuffer;
        std::shared_ptr<neon::Room> _room;
        std::chrono::high_resolution_clock::time_point _startTime;
        std::unique_ptr<Components> _components;

        std::shared_ptr<neon::Render> initRender();

        void initGameObjects();

    public:
        NeoneuronRender(const NeoneuronRender& other) = delete;

        explicit NeoneuronRender(const neon::vulkan::VKApplicationCreateInfo& createInfo);

        ~NeoneuronRender();

        [[nodiscard]] neon::Application& getApplication();

        [[nodiscard]] const neon::Application& getApplication() const;

        const neon::CMRCFileSystem getFileSystem() const;

        [[nodiscard]] const std::shared_ptr<neon::FrameBuffer>& getRenderFrameBuffer() const;

        [[nodiscard]] const std::shared_ptr<neon::Room>& getRoom() const;

        [[nodiscard]] NeuronScene& getNeuronScene();

        [[nodiscard]] const NeuronScene& getNeuronScene() const;

        [[nodiscard]] NeoneuronUI& getUI();

        [[nodiscard]] const NeoneuronUI& getUI() const;


        [[nodiscard]] CameraData& getCameraData();

        [[nodiscard]] const CameraData& getCameraData() const;

        float getCurrentTime() const;

        bool renderLoop();

        void focusScene() const;
    };
}

#endif //NEONEURONRENDER_H
