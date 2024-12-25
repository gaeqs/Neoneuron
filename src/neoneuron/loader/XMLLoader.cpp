//
// Created by gaeqs on 25/12/2024.
//

#include "XMLLoader.h"

#include <iostream>
#include <utility>

namespace neoneuron {
    XMLLoader::XMLLoader(neon::FileSystem* fileSystem, std::filesystem::path root, void* data, size_t size)
        : _fileSystem(fileSystem), _root(std::move(root)) {
        auto result = _doc.load_buffer(data, size);
        _valid = result.status == pugi::status_ok;
    }

    XMLLoader::XMLLoader(neon::FileSystem* fileSystem, std::filesystem::path root, std::istream& stream)
        : _fileSystem(fileSystem), _root(std::move(root)) {
        auto result = _doc.load(stream);
        _valid = result.status == pugi::status_ok;
    }

    neon::Result<PrototypeNeuron, std::string> XMLLoader::build(UID uid) const {
        if (!_valid) return {"Parser is not valid"};

        for (auto col: _doc.child("scene").child("morphology").child("columns").children("column")) {
            std::cout << "COL" << std::endl;
            for (auto min: col.children("minicolumn")) {
                std::cout << "MIN" << std::endl;
                for (auto neuron: min.children("neuron")) {
                    std::cout << "NEURON" << std::endl;
                    std::cout << "GID: " << neuron.attribute("gid").as_int(-1) << std::endl;
                    std::cout << "LAYER: " << neuron.attribute("layer").as_int(-1) << std::endl;
                }
            }
        }

        std::cout << _doc.child("scene") << std::endl;
        std::cout << _doc.child("scene").name() << std::endl;
        std::cout << _doc.child("scene").type() << std::endl;
        std::cout << _doc.child("patata") << std::endl;
        std::cout << _doc.child("patata").name() << std::endl;
        std::cout << _doc.child("patata").type() << std::endl;
        std::cout << _doc.child("patata").child("pototo") << std::endl;
        std::cout << _doc.child("patata").child("pototo").name() << std::endl;
        std::cout << _doc.child("patata").child("pototo").type() << std::endl;

        return {"Debug"};
    }
}
