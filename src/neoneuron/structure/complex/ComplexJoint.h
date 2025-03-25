//
// Created by gaeqs on 22/11/24.
//

#ifndef COMPLEXJOINT_H
#define COMPLEXJOINT_H
#include <vector>
#include <mnemea/Identifiable.h>

namespace neoneuron
{
    class ComplexNeuron;
    /**
    * Represents a joint of several segments.
    * The UID of this joint must represent the UID
    * of the parent segment.
    */
    class ComplexJoint : public mnemea::Identifiable
    {
        std::vector<mnemea::UID> _children;
        uint32_t _rotationIndex;

      public:
        ComplexJoint(mnemea::UID id, std::vector<mnemea::UID> children);

        [[nodiscard]] std::vector<mnemea::UID>& getChildren();

        [[nodiscard]] const std::vector<mnemea::UID>& getChildren() const;

        [[nodiscard]] uint32_t getRotationIndex() const;

        void computeRotationIndex(ComplexNeuron& neuron);
    };
} // namespace neoneuron

#endif //COMPLEXJOINT_H
