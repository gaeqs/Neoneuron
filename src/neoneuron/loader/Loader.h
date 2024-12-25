//
// Created by gaeqs on 8/10/24.
//

#ifndef LOADER_H
#define LOADER_H

#include <neon/util/Result.h>
#include <neoneuron/structure/prototype/PrototypeNeuron.h>

namespace neoneuron {
    class Loader {
    public:
        virtual ~Loader() = default;

        [[nodiscard]] virtual neon::Result<PrototypeNeuron, std::string> build(UID uid) const = 0;
    };
}

#endif //LOADER_H
