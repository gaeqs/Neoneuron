//
// Created by gaeqs on 23/12/24.
//

#include "DefinedProperty.h"

#include <utility>

namespace neoneuron {
    DefinedProperty::DefinedProperty(std::string name, std::type_index type, PropertyTarget target)
        : _name(std::move(name)), _type(type), _target(target), _editor(nullptr), _generator(nullptr) {}

    DefinedProperty::DefinedProperty(std::string name,
                                     std::type_index type,
                                     PropertyTarget target,
                                     std::string displayName)
        : _name(std::move(name)),
          _type(type),
          _target(target),
          _displayName(std::move(displayName)),
          _editor(nullptr),
          _generator(nullptr) {}

    DefinedProperty::DefinedProperty(std::string name, std::type_index type, PropertyTarget target,
                                     std::string displayName, EditorFunction editor, GeneratorFunction generator)
        : _name(std::move(name)),
          _type(type),
          _target(target),
          _displayName(std::move(displayName)),
          _editor(editor),
          _generator(generator) {}

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

    void DefinedProperty::setEditor(const EditorFunction& editor) {
        _editor = editor;
    }

    DefinedProperty::GeneratorFunction DefinedProperty::getGenerator() const {
        return _generator;
    }

    void DefinedProperty::setGenerator(const GeneratorFunction& generator) {
        _generator = generator;
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
