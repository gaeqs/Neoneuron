//
// Created by gaeqs on 8/10/24.
//

#include "Loader.h"

namespace neoneuron {
    LoaderBuilder::LoaderBuilder(std::string name, std::string displayName, Builder builder) :
        _name(std::move(name)), _displayName(std::move(displayName)), _builder(std::move(builder)) {}

    const std::string& LoaderBuilder::getName() const {
        return _name;
    }

    const std::string& LoaderBuilder::getDisplayName() const {
        return _displayName;
    }

    const LoaderBuilder::Builder& LoaderBuilder::getBuilder() const {
        return _builder;
    }

    std::unique_ptr<Loader> LoaderBuilder::build(neon::FileSystem* fileSystem, const neon::File& file) const {
        return _builder(fileSystem, file);
    }
}
