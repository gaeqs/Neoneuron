//
// Created by gaeqs on 23/12/24.
//

#ifndef DEFINEDPROPERTY_H
#define DEFINEDPROPERTY_H
#include <any>
#include <functional>
#include <string>
#include <typeindex>
#include <neoneuron/render/AbstractNeuronRepresentation.h>
#include <nlohmann/json.hpp>

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
        using EditorFunction =
            std::function<bool(std::any* property, const mindset::Neuron* neuron, const AbstractNeuronRepresentation* scene)>;

        using GeneratorFunction =
            std::function<std::any(const mindset::Neuron* neuron, const AbstractNeuronRepresentation* scene)>;

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
