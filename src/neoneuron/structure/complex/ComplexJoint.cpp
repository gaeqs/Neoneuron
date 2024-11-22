//
// Created by gaeqs on 22/11/24.
//

#include "ComplexJoint.h"


namespace neoneuron {
    ComplexJoint::ComplexJoint(UID id, std::vector<UID> children)
        : Identifiable(id),
          _children(std::move(children)) {}


    std::vector<UID>& ComplexJoint::getChildren() {
        return _children;
    }

    const std::vector<UID>& ComplexJoint::getChildren() const {
        return _children;
    }
}
