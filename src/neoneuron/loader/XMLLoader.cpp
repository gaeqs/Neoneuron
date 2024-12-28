//
// Created by gaeqs on 25/12/2024.
//

#include "XMLLoader.h"

#include <iostream>
#include <utility>

namespace {
    std::optional<neoneuron::UID> asUID(pugi::xml_attribute attr) {
        constexpr neoneuron::UID NOT_VALID = -1;
        neoneuron::UID id = attr.as_uint(NOT_VALID);
        if (id == NOT_VALID) return {};
        return id;
    }

    std::optional<std::string> asString(pugi::xml_attribute attr) {
        if (attr.empty()) return {};
        return attr.as_string();
    }

    neon::Result<std::vector<float>, std::string> split(const std::string& string, char delimiter) {
        std::vector<float> tokens;
        std::stringstream ss(string);

        std::string token;
        while (std::getline(ss, token, delimiter)) {
            try {
                tokens.push_back(std::stof(token));
            } catch (const std::invalid_argument& e) {
                return {"Invalid float! " + std::string(token)};
            }
            catch (const std::out_of_range& e) {
                return {"Number out of range! " + std::string(token)};
            }
        }

        return tokens;
    }
}

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
        auto scene = _doc.child("scene");
        if (!scene) return {"Scene not found"};

        std::unordered_map<UID, XMLNeuron> neurons;

        for (auto column: scene.child("morphology").child("columns").children("column")) {
            auto columnId = asUID(column.attribute("id"));

            for (auto miniColumn: column.children("minicolumn")) {
                auto miniColumnId = asUID(miniColumn.attribute("id"));

                for (auto neuron: miniColumn.children("neuron")) {
                    auto gid = asUID(neuron.attribute("gid"));
                    if (!gid.has_value()) return {"Neuron GID not found!"};

                    XMLNeuron xmlNeuron = {
                        .id = gid.value(),
                        .column = columnId,
                        .miniColumn = miniColumnId,
                        .layer = asUID(neuron.append_attribute("layer")),
                        .neuronType = asString(neuron.attribute("type"))
                    };

                    if (auto transform = neuron.child("transform").first_child()) {
                        std::string string = transform.value();
                        auto result = split(string, ',');
                        if (!result.isOk()) return {result.getError()};
                        auto floats = std::move(result.getResult());
                        if (floats.size() != 16) return {"Invalid matrix size."};

                        rush::Mat4f matrix([&floats](size_t c, size_t r) {
                            return floats[r * 4 + c];
                        });

                        xmlNeuron.transform = NeuronTransform(matrix);
                        std::cout << xmlNeuron.transform.getScale() << std::endl;
                        std::cout << xmlNeuron.transform.getRotation() << std::endl;
                        std::cout << xmlNeuron.transform.getRotation().rotationMatrix3() << std::endl;
                        std::cout << xmlNeuron.transform.getRotation().euler() << std::endl;
                        std::cout << xmlNeuron.transform.getPosition() << std::endl;
                        std::cout << "-----" << std::endl;
                    }
                }
            }
        }

        return {"Debug"};
    }
}
