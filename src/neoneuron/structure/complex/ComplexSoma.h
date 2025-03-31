//
// Created by gaeqs on 5/12/24.
//

#ifndef COMPLEXSOMA_H
#define COMPLEXSOMA_H

#include <vector>
#include <mindset/Identifiable.h>

namespace neoneuron
{
    class ComplexSoma : public mindset::Identifiable
    {
        std::vector<mindset::UID> _children;

      public:
        ComplexSoma(mindset::UID id, std::vector<mindset::UID> children);

        explicit ComplexSoma(mindset::UID id);

        [[nodiscard]] std::vector<mindset::UID>& getChildren();

        [[nodiscard]] const std::vector<mindset::UID>& getChildren() const;
    };
} // namespace neoneuron

#endif //COMPLEXSOMA_H
