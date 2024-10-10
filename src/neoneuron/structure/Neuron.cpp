//
// Created by gaeqs on 8/10/24.
//

#include "Neuron.h"

namespace neoneuron {
    Neuron::Neuron() : Identifiable(std::numeric_limits<UID>::max()) {}

    Neuron::Neuron(UID uid, const std::vector<NeuronSegment>& segments)
        : Identifiable(uid),
          _segments(segments) {
        for (size_t i = 0; i < _segments.size(); ++i) {
            _segmentsByUID.emplace(_segments[i].getId(), i);
        }
    }

    Neuron::Neuron(UID uid, std::vector<NeuronSegment>&& segments)
        : Identifiable(uid),
          _segments(std::move(segments)) {
        for (size_t i = 0; i < _segments.size(); ++i) {
            _segmentsByUID.emplace(_segments[i].getId(), i);
        }
    }

    const std::vector<NeuronSegment>& Neuron::getSegments() const {
        return _segments;
    }

    std::optional<const NeuronSegment*> Neuron::findSegment(UID uid) const {
        auto it = _segmentsByUID.find(uid);
        if (it == _segmentsByUID.end()) return {};
        return &_segments[it->second];
    }

    std::optional<NeuronSegment*> Neuron::findSegment(UID uid) {
        auto it = _segmentsByUID.find(uid);
        if (it == _segmentsByUID.end()) return {};
        return &_segments[it->second];
    }

    std::optional<size_t> Neuron::findSegmentIndex(UID uid) const {
        auto it = _segmentsByUID.find(uid);
        if (it == _segmentsByUID.end()) return {};
        return it->second;
    }
}
