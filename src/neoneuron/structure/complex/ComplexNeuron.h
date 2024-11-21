//
// Created by gaeqs on 8/10/24.
//

#ifndef COMPLEXNEURON_H
#define COMPLEXNEURON_H

#include <vector>
#include <unordered_map>
#include <neon/util/Result.h>

#include <neoneuron/structure/Identifiable.h>
#include <neoneuron/structure/complex/ComplexNeuronSegment.h>

#include <neoneuron/structure/prototype/PrototypeNeuron.h>

namespace neoneuron {
    class ComplexNeuron : public Identifiable {
        std::vector<ComplexNeuronSegment> _segments;
        std::unordered_map<UID, size_t> _segmentsByUID;
        rush::AABB<3, float> _boundingBox;

        void calculateBoundingBox();

    public:
        ComplexNeuron(const ComplexNeuron& other) = default;

        ComplexNeuron(ComplexNeuron&& other) noexcept;

        ComplexNeuron& operator=(const ComplexNeuron& other);

        ComplexNeuron& operator=(ComplexNeuron&& other) noexcept;

        ComplexNeuron();

        ComplexNeuron(UID uid, const std::vector<ComplexNeuronSegment>& segments);

        ComplexNeuron(UID uid, std::vector<ComplexNeuronSegment>&& segments);

        [[nodiscard]] rush::AABB<3, float> getBoundingBox() const;

        [[nodiscard]] const std::vector<ComplexNeuronSegment>& getSegments() const;

        std::optional<const ComplexNeuronSegment*> findSegment(UID uid) const;

        std::optional<ComplexNeuronSegment*> findSegment(UID uid);

        std::optional<size_t> findSegmentIndex(UID uid) const;


        // Static methods

        static neon::Result<ComplexNeuron, std::string> fromPrototype(const PrototypeNeuron& prototype);
    };
}

#endif //COMPLEXNEURON_H
