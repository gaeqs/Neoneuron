//
// Created by gaeqs on 22/11/24.
//

#ifndef COMPLEXJOINT_H
#define COMPLEXJOINT_H
#include <vector>
#include <mindset/Identifiable.h>

namespace neoneuron
{
    class ComplexNeuron;
    /**
    * Represents a joint of several segments.
    * The UID of this joint must represent the UID
    * of the parent segment.
    */
    class ComplexJoint : public mindset::Identifiable
    {
        std::vector<mindset::UID> _children;
        uint32_t _rotationIndex;

      public:
        ComplexJoint(mindset::UID id, std::vector<mindset::UID> children);

        [[nodiscard]] std::vector<mindset::UID>& getChildren();

        [[nodiscard]] const std::vector<mindset::UID>& getChildren() const;

        [[nodiscard]] uint32_t getRotationIndex() const;

        void computeRotationIndex(ComplexNeuron& neuron);
    };
} // namespace neoneuron

#endif //COMPLEXJOINT_H
