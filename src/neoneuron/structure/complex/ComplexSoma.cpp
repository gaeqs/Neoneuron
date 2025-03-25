//
// Created by gaeqs on 5/12/24.
//

#include "ComplexSoma.h"

namespace neoneuron
{
    ComplexSoma::ComplexSoma(mnemea::UID id, std::vector<mnemea::UID> children) :
        Identifiable(id),
        _children(std::move(children))
    {
    }

    ComplexSoma::ComplexSoma(mnemea::UID id) :
        Identifiable(id)
    {
    }

    std::vector<mnemea::UID>& ComplexSoma::getChildren()
    {
        return _children;
    }

    const std::vector<mnemea::UID>& ComplexSoma::getChildren() const
    {
        return _children;
    }
} // namespace neoneuron
