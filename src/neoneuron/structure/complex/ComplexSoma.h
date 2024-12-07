//
// Created by gaeqs on 5/12/24.
//

#ifndef COMPLEXSOMA_H
#define COMPLEXSOMA_H

#include <vector>
#include <neoneuron/structure/Identifiable.h>
#include <rush/vector/vec.h>

namespace neoneuron {
    class ComplexSoma : public Identifiable {
        std::vector<UID> _children;

    public:
        ComplexSoma(UID id, std::vector<UID> children);

        explicit ComplexSoma(UID id);

        [[nodiscard]] std::vector<UID>& getChildren();

        [[nodiscard]] const std::vector<UID>& getChildren() const;
    };
}


#endif //COMPLEXSOMA_H
