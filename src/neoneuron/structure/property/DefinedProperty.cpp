// Copyright (c) 2025. VG-Lab/URJC.
//
// Authors: Gael Rial Costas <gael.rial.costas@urjc.es>
//
// This file is part of Neoneuron <gitlab.gmrv.es/g.rial/neoneuron>
//
// This library is free software; you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License version 3.0 as published
// by the Free Software Foundation.
//
// This library is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this library; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

#include "DefinedProperty.h"

#include <utility>

namespace neoneuron
{
    DefinedProperty::DefinedProperty(std::string name, std::type_index type, PropertyTarget target) :
        _name(std::move(name)),
        _type(type),
        _target(target),
        _editor(nullptr),
        _generator(nullptr)
    {
    }

    DefinedProperty::DefinedProperty(std::string name, std::type_index type, PropertyTarget target,
                                     std::string displayName) :
        _name(std::move(name)),
        _type(type),
        _target(target),
        _displayName(std::move(displayName)),
        _editor(nullptr),
        _generator(nullptr)
    {
    }

    DefinedProperty::DefinedProperty(std::string name, std::type_index type, PropertyTarget target,
                                     std::string displayName, EditorFunction editor, GeneratorFunction generator) :
        _name(std::move(name)),
        _type(type),
        _target(target),
        _displayName(std::move(displayName)),
        _editor(std::move(editor)),
        _generator(std::move(generator))
    {
    }

    DefinedProperty::DefinedProperty(std::string name, std::type_index type, PropertyTarget target,
                                     std::string displayName, EditorFunction editor, GeneratorFunction generator,
                                     ToJsonFunction toJson, FromJsonFunction fromJson) :
        _name(std::move(name)),
        _type(type),
        _target(target),
        _displayName(std::move(displayName)),
        _editor(std::move(editor)),
        _generator(std::move(generator)),
        _toJson(std::move(toJson)),
        _fromJson(std::move(fromJson))
    {
    }

    const std::string& DefinedProperty::getName() const
    {
        return _name;
    }

    const std::type_index& DefinedProperty::getType() const
    {
        return _type;
    }

    PropertyTarget DefinedProperty::getTarget() const
    {
        return _target;
    }

    const std::string& DefinedProperty::getDisplayName() const
    {
        return _displayName;
    }

    void DefinedProperty::setDisplayName(const std::string& displayName)
    {
        _displayName = displayName;
    }

    DefinedProperty::EditorFunction DefinedProperty::getEditor() const
    {
        return _editor;
    }

    void DefinedProperty::setEditor(const EditorFunction& editor)
    {
        _editor = editor;
    }

    DefinedProperty::GeneratorFunction DefinedProperty::getGenerator() const
    {
        return _generator;
    }

    void DefinedProperty::setGenerator(const GeneratorFunction& generator)
    {
        _generator = generator;
    }

    DefinedProperty::ToJsonFunction DefinedProperty::getToJson() const
    {
        return _toJson;
    }

    void DefinedProperty::setToJson(const ToJsonFunction& toJson)
    {
        _toJson = toJson;
    }

    DefinedProperty::FromJsonFunction DefinedProperty::getFromJson() const
    {
        return _fromJson;
    }

    void DefinedProperty::setFromJson(const FromJsonFunction& fromJson)
    {
        _fromJson = fromJson;
    }

    bool DefinedProperty::operator==(const DefinedProperty& other) const
    {
        return _name == other._name;
    }

    bool DefinedProperty::operator!=(const DefinedProperty& other) const
    {
        return _name != other._name;
    }
} // namespace neoneuron

std::size_t std::hash<neoneuron::DefinedProperty>::operator()(const neoneuron::DefinedProperty& property) const noexcept
{
    return std::hash<std::string>{}(property.getName());
}
