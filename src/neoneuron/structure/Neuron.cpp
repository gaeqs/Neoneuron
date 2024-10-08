//
// Created by gaeqs on 8/10/24.
//

#include "Neuron.h"

namespace neoneuron {
    Neuron::Neuron() : Identifiable(std::numeric_limits<UID>::max()) {}

    Neuron::Neuron(UID uid, const std::vector<NeuronSegment>& segments)
        : Identifiable(uid),
          _segments(segments) {}

    Neuron::Neuron(UID uid, std::vector<NeuronSegment>&& segments)
        : Identifiable(uid),
          _segments(std::move(segments)) {}

    const std::vector<NeuronSegment>& Neuron::getSegments() const {
        return _segments;
    }
}
