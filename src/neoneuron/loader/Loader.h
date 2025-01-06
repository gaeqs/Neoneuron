//
// Created by gaeqs on 8/10/24.
//

#ifndef LOADER_H
#define LOADER_H

#include <neon/filesystem/FileSystem.h>
#include <neon/util/Result.h>
#include <neoneuron/structure/prototype/PrototypeNeuron.h>

namespace neoneuron {
    class Loader {
    public:
        virtual ~Loader() = default;

        [[nodiscard]] virtual neon::Result<std::vector<PrototypeNeuron>, std::string> build(UID uid) const = 0;
    };

    class LoaderBuilder {
    public:
        using Builder = std::function<std::unique_ptr<Loader>(neon::FileSystem*, const neon::File& file)>;

    private:
        std::string _name;
        std::string _displayName;
        Builder _builder;

    public:
        LoaderBuilder(std::string name, std::string displayName, Builder builder);

        [[nodiscard]] const std::string& getName() const;

        [[nodiscard]] const std::string& getDisplayName() const;

        [[nodiscard]] const Builder& getBuilder() const;

        std::unique_ptr<Loader> build(neon::FileSystem* fileSystem, const neon::File& file) const;
    };
}

#endif //LOADER_H
