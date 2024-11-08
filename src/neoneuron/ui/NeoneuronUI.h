//
// Created by gaeqs on 9/10/24.
//

#ifndef NEONEURONUI_H
#define NEONEURONUI_H

#include <neon/Neon.h>
#include <neon/util/component/ViewportComponent.h>

namespace neoneuron {

    class NeoneuronRender;

    class NeoneuronUI {

        neon::IdentifiableWrapper<neon::GameObject> _gameObject;
        neon::IdentifiableWrapper<neon::ViewportComponent> _viewport;

    public:
        NeoneuronUI() = default;

        explicit NeoneuronUI(NeoneuronRender* render);

        ~NeoneuronUI();

        NeoneuronUI& operator=(NeoneuronUI&& other) noexcept;

        [[nodiscard]] neon::IdentifiableWrapper<neon::ViewportComponent> getViewport() const;
    };
}

#endif //NEONEURONUI_H
