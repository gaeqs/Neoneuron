//
// Created by gaeqs on 14/11/24.
//

#include "PrototypeNeuronSegment.h"

namespace neoneuron {
    PrototypeNeuronSegment::PrototypeNeuronSegment(PrototypeNeuronSegment&& other) noexcept:
        Identifiable(std::move(other)),
        _properties(std::move(other._properties)) {}

    PrototypeNeuronSegment& PrototypeNeuronSegment::operator=(const PrototypeNeuronSegment& other) {
        if (this == &other)
            return *this;
        Identifiable::operator=(other);
        _properties = other._properties;
        return *this;
    }

    PrototypeNeuronSegment& PrototypeNeuronSegment::operator=(PrototypeNeuronSegment&& other) noexcept {
        if (this == &other)
            return *this;
        Identifiable::operator =(std::move(other));
        _properties = std::move(other._properties);
        return *this;
    }

    const std::unordered_map<UID, std::any>& PrototypeNeuronSegment::getProperties() const {
        return _properties;
    }

    PrototypeNeuronSegment::PrototypeNeuronSegment(UID uid)
        : Identifiable(uid) {}

    void PrototypeNeuronSegment::setPropertyAny(UID uid, std::any value) {
        _properties[uid] = std::move(value);
    }

    std::optional<std::any> PrototypeNeuronSegment::getPropertyAsAny(UID uid) const {
        auto it = _properties.find(uid);
        if (it == _properties.end()) return {};
        return it->second;
    }
}
