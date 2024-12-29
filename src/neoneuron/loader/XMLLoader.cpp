//
// Created by gaeqs on 25/12/2024.
//

#include "XMLLoader.h"

#include <iostream>
#include <utility>
#include <neoneuron/structure/prototype/NeuronProperties.h>

#include "SWCLoader.h"

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


    neon::Result<std::vector<neoneuron::UID>, std::string> splitUID(const std::string& string, char delimiter) {
        std::vector<neoneuron::UID> tokens;
        std::stringstream ss(string);

        std::string token;
        while (std::getline(ss, token, delimiter)) {
            try {
                tokens.push_back(std::stoi(token));
            } catch (const std::invalid_argument& e) {
                return {"Invalid int! " + std::string(token)};
            }
            catch (const std::out_of_range& e) {
                return {"Number out of range! " + std::string(token)};
            }
        }

        return tokens;
    }
}

namespace neoneuron {
    XMLLoader::XMLLoader(neon::FileSystem* fileSystem, const void* data, size_t size)
        : _fileSystem(fileSystem) {
        auto result = _doc.load_buffer(data, size);
        _valid = result.status == pugi::status_ok;
    }

    XMLLoader::XMLLoader(neon::FileSystem* fileSystem, std::istream& stream)
        : _fileSystem(fileSystem) {
        auto result = _doc.load(stream);
        _valid = result.status == pugi::status_ok;
    }

    neon::Result<std::vector<PrototypeNeuron>, std::string> XMLLoader::build(UID uid) const {
        if (!_valid) return {"Parser is not valid"};
        if (!_fileSystem) return {"Filesystem not set"};
        auto scene = _doc.child("scene").child("morphology");
        if (!scene) return {"Scene not found"};

        std::unordered_map<UID, XMLNeuron> xmlNeurons;

        for (auto column: scene.child("columns").children("column")) {
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
                        .neuronType = asString(neuron.attribute("type")),
                        .transform = {}
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
                    }

                    xmlNeurons.insert({gid.value(), std::move(xmlNeuron)});
                }
            }
        }

        std::vector<PrototypeNeuron> neurons;

        for (auto morpho: scene.child("neuronmorphologies").children("neuronmorphology")) {
            auto att = morpho.attribute("neurons");
            if (att.empty()) continue;

            auto fileAtt = morpho.attribute("swc");
            if (fileAtt.empty()) continue;

            auto result = splitUID(att.as_string(""), ',');
            if (!result.isOk()) return {result.getError()};
            auto uids = std::move(result.getResult());

            auto fileName = std::string(fileAtt.as_string(""));
            auto file = _fileSystem->readFile(fileName);
            if (!file.has_value()) return {"File not found: " + fileName};

            auto loader = SWCLoader(file.value());
            auto swcResult = loader.build(0);
            if (!swcResult.isOk())
                return {"Error loading SWC file '" + fileName + "': " + swcResult.getError()};
            if (swcResult.getResult().empty()) return {"No SWC neuron found"};
            auto swc = std::move(swcResult.getResult()[0]);

            for (UID id: uids) {
                auto it = xmlNeurons.find(id);
                if (it == xmlNeurons.end()) continue;
                auto& xml = it->second;
                auto prototype = swc;

                prototype.setId(xml.id);
                if (xml.transform.has_value()) {
                    prototype.defineAndSetProperty(PROPERTY_TRANSFORM, xml.transform.value());
                }
                neurons.push_back(std::move(prototype));
            }
        }

        return neurons;
    }
}
