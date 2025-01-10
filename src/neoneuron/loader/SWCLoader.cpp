//
// Created by gaeqs on 8/10/24.
//

#include "SWCLoader.h"

#include <sstream>
#include <unordered_map>
#include <neoneuron/structure/prototype/NeuronProperties.h>

namespace neoneuron {
    neon::Result<SWCLoader::SWCSegment, std::string> SWCLoader::toSegment(size_t lineIndex) const {
        SWCSegment segment;

        std::stringstream ss(_lines[lineIndex]);
        ss >> segment.id;
        ss >> segment.type;
        ss >> segment.end.x();
        ss >> segment.end.y();
        ss >> segment.end.z();
        ss >> segment.radius;
        ss >> segment.parent;

        if (ss.fail()) {
            return "StringStream error while parsing segment " + std::to_string(lineIndex)
                   + ". Bits: " + std::to_string(ss.rdstate()) + ".";
        }

        return segment;
    }

    SWCLoader::SWCLoader(const std::vector<std::string>& lines) : _lines(lines) {}

    SWCLoader::SWCLoader(std::vector<std::string>&& lines) : _lines(std::move(lines)) {}

    SWCLoader::SWCLoader(std::istream& stream) {
        std::string line;
        while (std::getline(stream, line)) {
            _lines.push_back(std::move(line));
        }
    }

    SWCLoader::SWCLoader(const neon::File& stream) : _lines(stream.readLines()) {}

    void SWCLoader::setPath(const std::filesystem::path& path) {
        _fileName = path.filename().string();
    }

    neon::Result<std::vector<PrototypeNeuron>, std::string> SWCLoader::build() const {
        std::unordered_map<UID, SWCSegment> prototypes;
        PrototypeNeuron neuron(0);

        // Define properties
        neuron.defineProperty(PROPERTY_TYPE, 0);
        neuron.defineProperty(PROPERTY_END, 1);
        neuron.defineProperty(PROPERTY_RADIUS, 2);
        neuron.defineProperty(PROPERTY_PARENT, 3);

        if (_fileName.has_value()) {
            neuron.defineProperty(PROPERTY_NAME, 4);
            neuron.setProperty(4, _fileName.value());
        }

        prototypes.reserve(_lines.size());
        neuron.reserveSpaceForSegments(_lines.size());

        for (size_t i = 0; i < _lines.size(); ++i) {
            auto line = _lines[i];
            if (line.starts_with("#") || line.empty()) continue;
            auto result = toSegment(i);
            if (!result.isOk()) return result.getError();
            prototypes.emplace(result.getResult().id, result.getResult());
        }

        for (auto& [id, prototype]: prototypes) {
            PrototypeNeuronSegment segment(id);
            segment.setProperty(0, static_cast<SegmentType>(prototype.type));
            segment.setProperty(1, prototype.end);
            segment.setProperty(2, prototype.radius);
            segment.setProperty(3, prototype.parent);
            neuron.addSegment(std::move(segment));
        }

        return std::vector{std::move(neuron)};
    }
}
