//
// Created by gaeqs on 4/12/24.
//

#include "NeoneuronApplication.h"

#include <neoneuron/loader/SWCLoader.h>
#include <neoneuron/loader/XMLLoader.h>

#include "NeoneuronDefaults.h"

namespace neoneuron {

    nlohmann::json NeoneuronApplication::loadSettings() {
        neon::DirectoryFileSystem dfs(std::filesystem::current_path());
        auto optional = dfs.readFile(CONFIG_FILE);
        if (!optional.has_value()) return "{}"_json;
        return optional->toJson().value_or(nlohmann::json());
    }

    NeoneuronApplication::NeoneuronApplication(neon::vulkan::VKApplicationCreateInfo renderCreateInfo)
        : _settings(loadSettings()),
          _render(this, std::move(renderCreateInfo)) {
        initDefaultProperties(_propertyStorage);
        initDefaultLoaders(_loaderStorage);
    }

    NeoneuronRender& NeoneuronApplication::getRender() {
        return _render;
    }

    const NeoneuronRender& NeoneuronApplication::getRender() const {
        return _render;
    }

    nlohmann::json& NeoneuronApplication::getSettings() {
        return _settings;
    }

    const nlohmann::json& NeoneuronApplication::getSettings() const {
        return _settings;
    }

    Storage<DefinedProperty>& NeoneuronApplication::getPropertyStorage() {
        return _propertyStorage;
    }

    const Storage<DefinedProperty>& NeoneuronApplication::getPropertyStorage() const {
        return _propertyStorage;
    }

    Storage<LoaderBuilder>& NeoneuronApplication::getLoaderStorage() {
        return _loaderStorage;
    }

    const Storage<LoaderBuilder>& NeoneuronApplication::getLoaderStorage() const {
        return _loaderStorage;
    }

    void NeoneuronApplication::registerSettingsListener(const hey::Listener<std::string>& listener) const {
        _settingsNodeChange.addListener(listener);
    }

    void NeoneuronApplication::unregisterSettingsListener(const hey::Listener<std::string>& listener) const {
        _settingsNodeChange.removeListener(listener);
    }

    void NeoneuronApplication::signalSettingsChange(std::string node) const {
        _settingsNodeChange.invoke(std::move(node));
    }

    void NeoneuronApplication::saveSettings() const {
        std::ofstream out(std::filesystem::current_path() / CONFIG_FILE);
        if (!out.good()) return;
        std::string data = _settings.dump(4);
        out.write(data.c_str(), data.size());
    }
}
