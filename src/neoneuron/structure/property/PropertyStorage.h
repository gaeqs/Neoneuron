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

    public:
        PropertyStorage();

        bool hasProperty(const std::string& name) const;

        DefinedProperty& getProperty(const std::string& name);

        const DefinedProperty& getProperty(const std::string& name) const;

        void defineProperty(DefinedProperty property);
    };
}


#endif //PROPERTYSTORAGE_H
