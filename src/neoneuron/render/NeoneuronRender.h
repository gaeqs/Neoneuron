//
// Created by gaeqs on 8/10/24.
//

#ifndef NEONEURONRENDER_H
#define NEONEURONRENDER_H

#include <neon/Neon.h>

#include <neoneuron/ui/NeoneuronUI.h>
#include <neoneuron/render/NeuronScene.h>


namespace neoneuron {
    class NeoneuronRender {
        neon::Application _application;
        std::shared_ptr<neon::FrameBuffer> _renderFrameBuffer;
        std::shared_ptr<neon::Room> _room;
        NeuronScene _neuronScene;
        NeoneuronUI _ui;

        std::shared_ptr<neon::Render> initRender();

        void initGameObjects() const;

    public:
        NeoneuronRender(const NeoneuronRender& other) = delete;

        explicit NeoneuronRender(const neon::vulkan::VKApplicationCreateInfo& createInfo);

        ~NeoneuronRender();

        [[nodiscard]] neon::Application& getApplication();

        [[nodiscard]] const neon::Application& getApplication() const;

        [[nodiscard]] const std::shared_ptr<neon::FrameBuffer>& getRenderFrameBuffer() const;

        [[nodiscard]] const std::shared_ptr<neon::Room>& getRoom() const;

        [[nodiscard]] NeuronScene& getNeuronScene();

        [[nodiscard]] const NeuronScene& getNeuronScene() const;

        bool renderLoop();
    };
}

#endif //NEONEURONRENDER_H
