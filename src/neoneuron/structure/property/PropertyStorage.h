//
// Created by gaeqs on 23/12/24.
//

#ifndef PROPERTYSTORAGE_H
#define PROPERTYSTORAGE_H
#include <string>
#include <unordered_map>

#include "DefinedProperty.h"

namespace neoneuron {
    class PropertyStorage {
        std::unordered_map<std::string, DefinedProperty> _properties;

        void addDefaultProperties();

    public:
        explicit PropertyStorage(bool defaultProperties = true);

        [[nodiscard]] bool hasProperty(const std::string& name) const;

        [[nodiscard]] const std::unordered_map<std::string, DefinedProperty>& getProperties() const;

        std::optional<DefinedProperty*> getProperty(const std::string& name);

        [[nodiscard]] std::optional<const DefinedProperty*> getProperty(const std::string& name) const;

        void defineProperty(DefinedProperty property);

        bool removeProperty(const std::string& name);
    };
}


#endif //PROPERTYSTORAGE_H
