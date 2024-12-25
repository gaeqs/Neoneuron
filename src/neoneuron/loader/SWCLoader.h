//
// Created by gaeqs on 8/10/24.
//

#ifndef SWCLOADER_H
#define SWCLOADER_H

#include <vector>
#include <string>

#include <neoneuron/loader/Loader.h>
#include <neoneuron/structure/prototype/PrototypeNeuron.h>

namespace neoneuron {
    class SWCLoader final : public Loader {
        struct SWCSegment {
            UID id;
            uint32_t type;
            rush::Vec3f end;
            float radius;
            int64_t parent;
        };

        std::optional<std::string> _fileName;
        std::vector<std::string> _lines;

        [[nodiscard]] neon::Result<SWCSegment, std::string> toSegment(size_t lineIndex) const;

    public:
        explicit SWCLoader(const std::vector<std::string>& lines);

        explicit SWCLoader(std::vector<std::string>&& lines);

        explicit SWCLoader(std::istream& stream);

        ~SWCLoader() override = default;

        void setFileName(std::string fileName);

        [[nodiscard]] neon::Result<PrototypeNeuron, std::string> build(UID uid) const override;
    };
}


#endif //SWCLOADER_H
