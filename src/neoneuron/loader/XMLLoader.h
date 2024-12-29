//
// Created by gaeqs on 25/12/2024.
//

#ifndef XMLLOADER_H
#define XMLLOADER_H

#include <vector>
#include <filesystem>
#include <string>

#include <pugixml.hpp>

#include <neon/filesystem/FileSystem.h>

#include <neoneuron/loader/Loader.h>
#include <neoneuron/structure/NeuronTransform.h>

namespace neoneuron {
    class XMLLoader final : public Loader {
        struct XMLNeuron {
            UID id;
            std::optional<UID> column;
            std::optional<UID> miniColumn;
            std::optional<UID> layer;
            std::optional<std::string> neuronType;
            std::optional<NeuronTransform> transform;
        };

        neon::FileSystem* _fileSystem;

        pugi::xml_document _doc;
        bool _valid;

    public:
        XMLLoader(neon::FileSystem* fileSystem, const void* data, size_t size);

        XMLLoader(neon::FileSystem* fileSystem, std::istream& stream);

        [[nodiscard]] neon::Result<std::vector<PrototypeNeuron>, std::string> build(UID uid) const override;
    };
}


#endif //XMLLOADER_H
