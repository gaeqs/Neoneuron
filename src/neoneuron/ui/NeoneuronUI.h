//
// Created by gaeqs on 9/10/24.
//

#ifndef NEONEURONUI_H
#define NEONEURONUI_H

#include <neon/Neon.h>

namespace neoneuron {
    class NeoneuronUI {
        neon::IdentifiableWrapper<neon::GameObject> _gameObject;

    public:
        NeoneuronUI() = default;

        explicit NeoneuronUI(neon::Room* room);

        ~NeoneuronUI();

        NeoneuronUI& operator=(NeoneuronUI&& other) noexcept;
    };
}

#endif //NEONEURONUI_H
