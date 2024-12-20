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

    std::optional<UID> PrototypeNeuron::getPropertyUID(const std::string& name) const {
        auto it = _properties.find(name);
        if (it == _properties.end()) return {};
        return it->second;
    }

    std::unordered_map<std::string, UID>& PrototypeNeuron::getPropertiesUID() {
        return _properties;
    }

    const std::unordered_map<std::string, UID>& PrototypeNeuron::getPropertiesUID() const {
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

    void PrototypeNeuron::setPropertyAny(UID uid, std::any value) {
        _globalProperties[uid] = std::move(value);
    }

    std::optional<std::any> PrototypeNeuron::getPropertyAsAny(UID uid) const {
        auto it = _globalProperties.find(uid);
        if (it == _globalProperties.end()) return {};
        return it->second;
    }
}
