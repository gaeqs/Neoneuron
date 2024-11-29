//
// Created by gaeqs on 22/11/24.
//

#ifndef COMPLEXJOINT_H
#define COMPLEXJOINT_H
#include <vector>
#include <neoneuron/structure/Identifiable.h>

namespace neoneuron {
    class ComplexNeuron;
    /**
    * Represents a joint of several segments.
    * The UID of this joint must represent the UID
    * of the parent segment.
    */
    class ComplexJoint : public Identifiable {
        std::vector<UID> _children;
        uint32_t _rotationIndex;

    public:
        ComplexJoint(UID id, std::vector<UID> children);

        [[nodiscard]] std::vector<UID>& getChildren();

        [[nodiscard]] const std::vector<UID>& getChildren() const;

        [[nodiscard]] uint32_t getRotationIndex() const;

        void computeRotationIndex(ComplexNeuron& neuron);
    };
}


#endif //COMPLEXJOINT_H
