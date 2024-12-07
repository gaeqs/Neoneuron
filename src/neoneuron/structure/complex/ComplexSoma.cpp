//
// Created by gaeqs on 5/12/24.
//

#include "ComplexSoma.h"

namespace neoneuron {
    ComplexSoma::ComplexSoma(UID id, std::vector<UID> children)
        : Identifiable(id), _children(std::move(children)) {}

    ComplexSoma::ComplexSoma(UID id)
        : Identifiable(id) {}


    std::vector<UID>& ComplexSoma::getChildren() {
        return _children;
    }

    const std::vector<UID>& ComplexSoma::getChildren() const {
        return _children;
    }
}
