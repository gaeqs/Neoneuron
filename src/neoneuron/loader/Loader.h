//
// Created by gaeqs on 8/10/24.
//

#ifndef LOADER_H
#define LOADER_H

#include <neon/assimp/AssimpLoader.h>
#include <neoneuron/structure/Neuron.h>

namespace neoneuron {
    class Loader {
    public:
        virtual ~Loader() = default;

        [[nodiscard]] virtual neon::Result<Neuron, std::string> build(UID uid) const = 0;
    };
}

#endif //LOADER_H
