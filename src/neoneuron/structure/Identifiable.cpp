//
// Created by gaeqs on 8/10/24.
//

#include "Identifiable.h"

namespace neoneuron {
    Identifiable::Identifiable(UID id): _id(id) {}

    UID Identifiable::getId() const {
        return _id;
    }

    void Identifiable::setId(UID id) {
        _id = id;
    }
}
