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

    const std::string XML_LOADER_NAME = "neoneuron:xml";
    const std::string XML_LOADER_DISPLAY_NAME = "XML Loader";

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

        XMLLoader(neon::FileSystem* fileSystem, const neon::File& file);

        void setPath(const std::filesystem::path& path) override;

        [[nodiscard]] neon::Result<std::vector<PrototypeNeuron>, std::string> build() const override;
    };
}


#endif //XMLLOADER_H
