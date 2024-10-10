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
        NeoneuronUI _ui;
        NeuronScene _neuronScene;

        std::shared_ptr<neon::Render> initRender();

    public:
        NeoneuronRender(const NeoneuronRender& other) = delete;

        explicit NeoneuronRender(const neon::vulkan::VKApplicationCreateInfo& createInfo);

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
