//
// Created by gaeqs on 5/12/24.
//

#include "ComplexSoma.h"

namespace neoneuron
{
    ComplexSoma::ComplexSoma(mindset::UID id, std::vector<mindset::UID> children) :
        Identifiable(id),
        _children(std::move(children))
    {
    }

    ComplexSoma::ComplexSoma(mindset::UID id) :
        Identifiable(id)
    {
    }

    std::vector<mindset::UID>& ComplexSoma::getChildren()
    {
        return _children;
    }

    const std::vector<mindset::UID>& ComplexSoma::getChildren() const
    {
        return _children;
    }
} // namespace neoneuron
