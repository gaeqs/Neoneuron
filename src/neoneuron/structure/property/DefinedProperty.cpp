//
// Created by gaeqs on 23/12/24.
//

#include "DefinedProperty.h"

#include <utility>

namespace neoneuron {
    DefinedProperty::DefinedProperty(std::string name, std::type_index type, PropertyTarget target)
        : _name(std::move(name)), _type(type), _target(target), _editor(nullptr) {}

    DefinedProperty::DefinedProperty(std::string name,
                                     std::type_index type,
                                     PropertyTarget target,
                                     std::string displayName)
        : _name(std::move(name)),
          _type(type),
          _target(target),
          _displayName(std::move(displayName)),
          _editor(nullptr) {}

    const std::string& DefinedProperty::getName() const {
        return _name;
    }

    const std::type_index& DefinedProperty::getType() const {
        return _type;
    }

    PropertyTarget DefinedProperty::getTarget() const {
        return _target;
    }

    const std::string& DefinedProperty::getDisplayName() const {
        return _displayName;
    }

    void DefinedProperty::setDisplayName(const std::string& displayName) {
        _displayName = displayName;
    }

    DefinedProperty::EditorFunction DefinedProperty::getEditor() const {
        return _editor;
    }

    void DefinedProperty::setEditor(EditorFunction editor) {
        _editor = editor;
    }

    bool DefinedProperty::operator==(const DefinedProperty& other) const {
        return _name == other._name;
    }

    bool DefinedProperty::operator!=(const DefinedProperty& other) const {
        return _name != other._name;
    }
}

std::size_t std::hash<neoneuron::DefinedProperty>::operator(
)(const neoneuron::DefinedProperty& property) const noexcept {
    return std::hash<std::string>{}(property.getName());
}
