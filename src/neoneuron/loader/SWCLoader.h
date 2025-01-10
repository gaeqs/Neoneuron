//
// Created by gaeqs on 8/10/24.
//

#ifndef SWCLOADER_H
#define SWCLOADER_H

#include <vector>
#include <string>
#include <neon/filesystem/File.h>

#include <rush/rush.h>

#include <neoneuron/loader/Loader.h>
#include <neoneuron/structure/prototype/PrototypeNeuron.h>

namespace neoneuron {
    const std::string SWC_LOADER_NAME = "neoneuron:swc";
    const std::string SWC_LOADER_DISPLAY_NAME = "SWC Loader";

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

        explicit SWCLoader(const neon::File& stream);

        ~SWCLoader() override = default;

        void setPath(const std::filesystem::path& path) override;

        [[nodiscard]] neon::Result<std::vector<PrototypeNeuron>, std::string> build() const override;
    };
}


#endif //SWCLOADER_H
