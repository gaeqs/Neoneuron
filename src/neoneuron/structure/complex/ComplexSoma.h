//
// Created by gaeqs on 5/12/24.
//

#ifndef COMPLEXSOMA_H
#define COMPLEXSOMA_H

#include <vector>
#include <mnemea/Identifiable.h>

namespace neoneuron
{
    class ComplexSoma : public mnemea::Identifiable
    {
        std::vector<mnemea::UID> _children;

      public:
        ComplexSoma(mnemea::UID id, std::vector<mnemea::UID> children);

        explicit ComplexSoma(mnemea::UID id);

        [[nodiscard]] std::vector<mnemea::UID>& getChildren();

        [[nodiscard]] const std::vector<mnemea::UID>& getChildren() const;
    };
} // namespace neoneuron

#endif //COMPLEXSOMA_H
