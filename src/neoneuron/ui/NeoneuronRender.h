//
// Created by gaeqs on 8/10/24.
//

#ifndef NEONEURONRENDER_H
#define NEONEURONRENDER_H

#include <neon/Neon.h>

#include "NeoneuronUI.h"

namespace neoneuron {
    class NeoneuronRender {
        neon::Application _application;
        std::shared_ptr<neon::Room> _room;
        std::unique_ptr<NeoneuronUI> _ui;

        std::shared_ptr<neon::Render> initRender();

    public:
        explicit NeoneuronRender(const neon::vulkan::VKApplicationCreateInfo& createInfo);

        [[nodiscard]] neon::Application& getApplication();

        [[nodiscard]] const neon::Application& getApplication() const;

        bool renderLoop();
    };
}

#endif //NEONEURONRENDER_H
