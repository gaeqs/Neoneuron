//
// Created by gaeqs on 8/10/24.
//

#ifndef SWCLOADER_H
#define SWCLOADER_H

#include <vector>
#include <string>
#include <fstream>

#include <neoneuron/loader/Loader.h>

namespace neoneuron {
    class SWCLoader final : public Loader {
        struct SegmentPrototype {
            UID id;
            uint8_t type;
            rush::Vec3f end;
            float radius;
            int64_t parent;
        };

        std::vector<std::string> _lines;

        [[nodiscard]] neon::Result<SegmentPrototype, std::string> toSegment(size_t lineIndex) const;

    public:
        explicit SWCLoader(const std::vector<std::string>& lines);

        explicit SWCLoader(std::vector<std::string>&& lines);

        explicit SWCLoader(std::istream& stream);

        ~SWCLoader() override = default;

        [[nodiscard]] neon::Result<Neuron, std::string> build(UID uid) const override;
    };
}


#endif //SWCLOADER_H
