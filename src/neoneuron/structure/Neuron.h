//
// Created by gaeqs on 8/10/24.
//

#ifndef NEURON_H
#define NEURON_H

#include <vector>
#include <unordered_map>

#include <neoneuron/structure/Identifiable.h>
#include <neoneuron/structure/NeuronSegment.h>

namespace neoneuron {
    class Neuron : public Identifiable {
        std::vector<NeuronSegment> _segments;
        std::unordered_map<UID, size_t> _segmentsByUID;
        rush::AABB<3, float> _boundingBox;

        void calculateBoundingBox();

    public:
        Neuron();

        Neuron(UID uid, const std::vector<NeuronSegment>& segments);

        Neuron(UID uid, std::vector<NeuronSegment>&& segments);

        [[nodiscard]] rush::AABB<3, float> boundingBox() const;

        [[nodiscard]] const std::vector<NeuronSegment>& getSegments() const;

        std::optional<const NeuronSegment*> findSegment(UID uid) const;

        std::optional<NeuronSegment*> findSegment(UID uid);

        std::optional<size_t> findSegmentIndex(UID uid) const;
    };
}

#endif //NEURON_H
