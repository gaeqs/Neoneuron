//
// Created by gaeqs on 9/10/24.
//

#ifndef NEONEURONUI_H
#define NEONEURONUI_H

#include <hey/Listener.h>
#include <neon/Neon.h>
#include <neon/util/component/ViewportComponent.h>

namespace neoneuron {
    class NeoneuronRender;

    class NeoneuronUI {
        NeoneuronRender* _render;
        neon::IdentifiableWrapper<neon::GameObject> _gameObject;
        neon::IdentifiableWrapper<neon::ViewportComponent> _viewport;
        hey::Listener<std::string> _debugKeyListener;

        void initStyle(NeoneuronRender* render);

        void initDebugToggle();

    public:
        NeoneuronUI() = default;

        explicit NeoneuronUI(NeoneuronRender* render);

        ~NeoneuronUI();

        NeoneuronUI& operator=(NeoneuronUI&& other) noexcept;

        [[nodiscard]] neon::IdentifiableWrapper<neon::ViewportComponent> getViewport() const;
    };
}

#endif //NEONEURONUI_H
