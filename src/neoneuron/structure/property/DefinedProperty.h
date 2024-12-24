//
// Created by gaeqs on 23/12/24.
//

#ifndef DEFINEDPROPERTY_H
#define DEFINEDPROPERTY_H
#include <any>
#include <functional>
#include <string>
#include <typeindex>
#include <neoneuron/render/AbstractNeuronScene.h>
#include <neoneuron/structure/prototype/PrototypeNeuron.h>

namespace neoneuron {
    enum class PropertyTarget {
        NEURON,
        SEGMENT,
        BOTH
    };

    class DefinedProperty {
    public:
        using EditorFunction = std::function<bool(std::any* property,
                                                  const PrototypeNeuron* neuron,
                                                  const AbstractNeuronScene* scene)>;

        using GeneratorFunction = std::function<std::any(const PrototypeNeuron* neuron,
                                                         const AbstractNeuronScene* scene)>;

    private:
        std::string _name;
        std::type_index _type;
        PropertyTarget _target;
        std::string _displayName;
        EditorFunction _editor;
        GeneratorFunction _generator;

    public:
        DefinedProperty(std::string name, std::type_index type, PropertyTarget target);

        DefinedProperty(std::string name, std::type_index type, PropertyTarget target, std::string displayName);

        DefinedProperty(std::string name, std::type_index type, PropertyTarget target, std::string displayName,
            EditorFunction editor, GeneratorFunction generator);

        [[nodiscard]] const std::string& getName() const;

        [[nodiscard]] const std::type_index& getType() const;

        [[nodiscard]] PropertyTarget getTarget() const;

        [[nodiscard]] const std::string& getDisplayName() const;

        void setDisplayName(const std::string& displayName);

        [[nodiscard]] EditorFunction getEditor() const;

        void setEditor(const EditorFunction& editor);

        [[nodiscard]] GeneratorFunction getGenerator() const;

        void setGenerator(const GeneratorFunction& generator);

        bool operator==(const DefinedProperty& other) const;

        bool operator!=(const DefinedProperty& other) const;
    };
}

template<>
struct std::hash<neoneuron::DefinedProperty> {
    std::size_t operator()(const neoneuron::DefinedProperty& property) const noexcept;
};


#endif //DEFINEDPROPERTY_H
