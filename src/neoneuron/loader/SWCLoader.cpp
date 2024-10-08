//
// Created by gaeqs on 8/10/24.
//

#include "SWCLoader.h"

#include <unordered_map>

namespace neoneuron {
    neon::Result<SWCLoader::SegmentPrototype, std::string> SWCLoader::toSegment(size_t lineIndex) const {
        SegmentPrototype segment;

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
            if (line.starts_with("#") || line.empty()) continue;
            _lines.push_back(std::move(line));
        }
    }

    neon::Result<Neuron, std::string> SWCLoader::build(UID uid) const {
        std::unordered_map<UID, SegmentPrototype> prototypes;
        std::vector<NeuronSegment> segments;

        prototypes.reserve(_lines.size());
        prototypes.reserve(segments.size());

        for (size_t i = 0; i < _lines.size(); ++i) {
            auto result = toSegment(i);
            if (!result.isOk()) return result.getError();
            prototypes.emplace(result.getResult().id, result.getResult());
        }

        for (auto& [id, prototype]: prototypes) {
            SegmentPrototype* parent = nullptr;
            if (prototype.parent >= 0) {
                UID parentUID = static_cast<UID>(prototype.parent);
                auto it = prototypes.find(parentUID);
                if (it == prototypes.end()) {
                    return "Error while parsing segment " + std::to_string(id) +
                           ". Parent " + std::to_string(parentUID) + " not found.";
                }
                parent = &it->second;
            }

            rush::Vec3f start = parent == nullptr ? prototype.end : parent->end;

            segments.emplace_back(
                prototype.id,
                static_cast<SegmentType>(prototype.type),
                start,
                prototype.end,
                prototype.radius,
                parent == nullptr ? std::optional<UID>() : parent->id
            );
        }


        return Neuron(uid, std::move(segments));
    }
}
