//
// Created by gaeqs on 23/12/24.
//

#include "PropertyStorage.h"

#include <neoneuron/structure/NeuronTransform.h>
#include <neoneuron/structure/prototype/NeuronProperties.h>

#include "DefaultProperties.h"


void neoneuron::PropertyStorage::addDefaultProperties() {
    defineProperty({
        PROPERTY_TRANSFORM,
        typeid(NeuronTransform),
        PropertyTarget::NEURON,
        "Transform",
        property::transformEditor,
        property::transformGenerator
    });
    defineProperty({
        PROPERTY_NAME,
        typeid(std::string),
        PropertyTarget::NEURON,
        "Name",
        property::nameEditor,
        property::nameGenerator
    });
}

neoneuron::PropertyStorage::PropertyStorage(bool defaultProperties) {
    if (defaultProperties) {
        addDefaultProperties();
    }
}

bool neoneuron::PropertyStorage::hasProperty(const std::string& name) const {
    return _properties.contains(name);
}

const std::unordered_map<std::string, neoneuron::DefinedProperty>& neoneuron::PropertyStorage::getProperties() const {
    return _properties;
}

std::optional<neoneuron::DefinedProperty*> neoneuron::PropertyStorage::getProperty(const std::string& name) {
    auto it = _properties.find(name);
    if (it == _properties.end()) return {};
    return &it->second;
}

std::optional<const neoneuron::DefinedProperty*>
neoneuron::PropertyStorage::getProperty(const std::string& name) const {
    auto it = _properties.find(name);
    if (it == _properties.end()) return {};
    return &it->second;
}

void neoneuron::PropertyStorage::defineProperty(DefinedProperty property) {
    _properties.insert({property.getName(), std::move(property)});
}

bool neoneuron::PropertyStorage::removeProperty(const std::string& name) {
    return _properties.erase(name) > 0;
}
