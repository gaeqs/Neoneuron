//
// Created by gaeqs on 14/11/24.
//

#include "PrototypeNeuron.h"

namespace neoneuron {
    PrototypeNeuron::PrototypeNeuron(UID id)
        : Identifiable(id) {}

    void PrototypeNeuron::defineProperty(std::string name, UID id) {
        _properties[std::move(name)] = id;
    }

    std::optional<UID> PrototypeNeuron::getProperty(const std::string& name) const {
        auto it = _properties.find(name);
        if (it == _properties.end()) return {};
        return it->second;
    }

    std::unordered_map<std::string, UID>& PrototypeNeuron::getProperties() {
        return _properties;
    }

    const std::unordered_map<std::string, UID>& PrototypeNeuron::getProperties() const {
        return _properties;
    }

    void PrototypeNeuron::addSegment(PrototypeNeuronSegment segment) {
        _segments.push_back(std::move(segment));
    }

    std::vector<PrototypeNeuronSegment>& PrototypeNeuron::getSegments() {
        return _segments;
    }

    const std::vector<PrototypeNeuronSegment>& PrototypeNeuron::getSegments() const {
        return _segments;
    }

    void PrototypeNeuron::reserveSpaceForSegments(size_t space) {
        _segments.reserve(space);
    }
}
