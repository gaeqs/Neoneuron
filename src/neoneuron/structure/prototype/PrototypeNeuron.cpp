//
// Created by gaeqs on 14/11/24.
//

#include "PrototypeNeuron.h"

#include <ranges>

namespace neoneuron {
    PrototypeNeuron::PrototypeNeuron(UID id)
        : Identifiable(id) {}

    UID PrototypeNeuron::defineProperty(std::string name) {
        if (auto found = getPropertyUID(name); found.has_value()) return found.value();

        UID maxId = 0;
        for (auto id: _properties | std::views::values) {
            maxId = std::max(id, maxId);
        }

        UID id = maxId + 1;
        _properties[name] = id;
        _propertiesNames[id] = std::move(name);

        return id;
    }

    void PrototypeNeuron::defineProperty(std::string name, UID id) {
        _properties[name] = id;
        _propertiesNames[id] = std::move(name);
    }

    bool PrototypeNeuron::isPropertyDefined(const std::string& name) {
        return _properties.contains(name);
    }

    std::optional<UID> PrototypeNeuron::getPropertyUID(const std::string& name) const {
        auto it = _properties.find(name);
        if (it == _properties.end()) return {};
        return it->second;
    }

    const std::unordered_map<std::string, UID>& PrototypeNeuron::getPropertiesUIDs() const {
        return _properties;
    }

    const std::unordered_map<UID, std::string>& PrototypeNeuron::getPropertiesNames() const {
        return _propertiesNames;
    }

    const std::unordered_map<UID, std::any>& PrototypeNeuron::getGlobalProperties() const {
        return _globalProperties;
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

    bool PrototypeNeuron::deleteGlobalProperty(const std::string& name) {
        auto uid = getPropertyUID(name);
        if (!uid.has_value()) return false;
        return _globalProperties.erase(uid.value()) > 0;
    }

    bool PrototypeNeuron::deleteGlobalProperty(UID uid) {
        return _globalProperties.erase(uid) > 0;
    }

    std::optional<std::any> PrototypeNeuron::getPropertyAsAny(const std::string& name) const {
        auto uid = getPropertyUID(name);
        if (!uid.has_value()) return {};
        return getPropertyAsAny(uid.value());
    }

    std::optional<std::any> PrototypeNeuron::getPropertyAsAny(UID uid) const {
        auto it = _globalProperties.find(uid);
        if (it == _globalProperties.end()) return {};
        return it->second;
    }
}
