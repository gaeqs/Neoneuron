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

namespace neoneuron {
    class XMLLoader final : public Loader {
        neon::FileSystem* _fileSystem;
        std::filesystem::path _root;

        pugi::xml_document _doc;
        bool _valid;

    public:
        XMLLoader(neon::FileSystem* fileSystem, std::filesystem::path  root, void* data, size_t size);

        XMLLoader(neon::FileSystem* fileSystem, std::filesystem::path  root, std::istream& stream);

        [[nodiscard]] neon::Result<PrototypeNeuron, std::string> build(UID uid) const override;
    };
}


#endif //XMLLOADER_H
