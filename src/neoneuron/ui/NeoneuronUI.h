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
        explicit NeoneuronUI(neon::Room* room);

        ~NeoneuronUI();
    };
}

#endif //NEONEURONUI_H
