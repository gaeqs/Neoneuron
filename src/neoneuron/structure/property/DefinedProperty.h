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

#ifndef DEFINEDPROPERTY_H
#define DEFINEDPROPERTY_H
#include <any>
#include <functional>
#include <string>
#include <typeindex>
#include <neoneuron/render/AbstractNeuronRepresentation.h>
#include <nlohmann/json.hpp>

#include <mindset/mindset.h>

namespace neoneuron
{
    enum class PropertyTarget
    {
        NEURON,
        SEGMENT,
        BOTH
    };

    class DefinedProperty
    {
      public:
        using EditorFunction = std::function<bool(std::any* property, const mindset::Neuron* neuron)>;

        using GeneratorFunction = std::function<std::any(const mindset::Neuron* neuron)>;

        using ToJsonFunction = std::function<nlohmann::json(const std::any& any)>;
        using FromJsonFunction = std::function<std::any(const nlohmann::json& json)>;

      private:
        std::string _name;
        std::type_index _type;
        PropertyTarget _target;
        std::string _displayName;
        EditorFunction _editor;
        GeneratorFunction _generator;
        ToJsonFunction _toJson;
        FromJsonFunction _fromJson;

      public:
        DefinedProperty(std::string name, std::type_index type, PropertyTarget target);

        DefinedProperty(std::string name, std::type_index type, PropertyTarget target, std::string displayName);

        DefinedProperty(std::string name, std::type_index type, PropertyTarget target, std::string displayName,
                        EditorFunction editor, GeneratorFunction generator);

        DefinedProperty(std::string name, std::type_index type, PropertyTarget target, std::string displayName,
                        EditorFunction editor, GeneratorFunction generator, ToJsonFunction toJson,
                        FromJsonFunction fromJson);

        [[nodiscard]] const std::string& getName() const;

        [[nodiscard]] const std::type_index& getType() const;

        [[nodiscard]] PropertyTarget getTarget() const;

        [[nodiscard]] const std::string& getDisplayName() const;

        void setDisplayName(const std::string& displayName);

        [[nodiscard]] EditorFunction getEditor() const;

        void setEditor(const EditorFunction& editor);

        [[nodiscard]] GeneratorFunction getGenerator() const;

        void setGenerator(const GeneratorFunction& generator);

        [[nodiscard]] ToJsonFunction getToJson() const;

        void setToJson(const ToJsonFunction& toJson);

        [[nodiscard]] FromJsonFunction getFromJson() const;

        void setFromJson(const FromJsonFunction& fromJson);

        bool operator==(const DefinedProperty& other) const;

        bool operator!=(const DefinedProperty& other) const;
    };
} // namespace neoneuron

template<>
struct std::hash<neoneuron::DefinedProperty>
{
    std::size_t operator()(const neoneuron::DefinedProperty& property) const noexcept;
};

#endif //DEFINEDPROPERTY_H
