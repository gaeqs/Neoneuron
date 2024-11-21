//
// Created by gaeqs on 8/10/24.
//

#ifndef NEURON_H
#define NEURON_H

#include <vector>
#include <unordered_map>
#include <neon/util/Result.h>

#include <neoneuron/structure/Identifiable.h>
#include <neoneuron/structure/simple/SimpleNeuronSegment.h>

#include <neoneuron/structure/prototype/PrototypeNeuron.h>

namespace neoneuron {
    class SimpleNeuron : public Identifiable {
        std::vector<SimpleNeuronSegment> _segments;
        std::unordered_map<UID, size_t> _segmentsByUID;
        rush::AABB<3, float> _boundingBox;

        void calculateBoundingBox();

    public:
        SimpleNeuron(const SimpleNeuron& other) = default;

        SimpleNeuron(SimpleNeuron&& other) noexcept;

        SimpleNeuron& operator=(const SimpleNeuron& other);

        SimpleNeuron& operator=(SimpleNeuron&& other) noexcept;

        SimpleNeuron();

        SimpleNeuron(UID uid, const std::vector<SimpleNeuronSegment>& segments);

        SimpleNeuron(UID uid, std::vector<SimpleNeuronSegment>&& segments);

        [[nodiscard]] rush::AABB<3, float> getBoundingBox() const;

        [[nodiscard]] const std::vector<SimpleNeuronSegment>& getSegments() const;

        std::optional<const SimpleNeuronSegment*> findSegment(UID uid) const;

        std::optional<SimpleNeuronSegment*> findSegment(UID uid);

        std::optional<size_t> findSegmentIndex(UID uid) const;


        // Static methods

        static neon::Result<SimpleNeuron, std::string> fromPrototype(const PrototypeNeuron& prototype);
    };
}

#endif //NEURON_H
