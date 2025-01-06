//
// Created by gaeqs on 4/12/24.
//

#ifndef NEONEURONAPPLICATION_H
#define NEONEURONAPPLICATION_H
#include <neoneuron/loader/Loader.h>
#include <neoneuron/render/NeoneuronRender.h>
#include <neoneuron/structure/Storage.h>
#include <neoneuron/structure/property/DefinedProperty.h>

namespace neoneuron {
    class NeoneuronApplication {
    public:
        inline static const std::string CONFIG_FILE = "neoneuron.json";
        static inline const std::string SETTINGS_THEME = "theme";
        static inline const std::string SETTINGS_FONT_SIZE = "font_size";

        static inline const std::string SETTINGS_TOOL_GLOBAL_PARAMETERS = "tool_global_parameters";
        static inline const std::string SETTINGS_TOOL_DEBUG = "tool_debug";
        static inline const std::string SETTINGS_TOOL_DEMO = "tool_demo";

    private:
        nlohmann::json _settings;
        hey::Observable<std::string> _settingsNodeChange;
        NeoneuronRender _render;
        Storage<DefinedProperty> _propertyStorage;
        Storage<LoaderBuilder> _loaderStorage;

        static nlohmann::json loadSettings();

    public:
        explicit NeoneuronApplication(neon::vulkan::VKApplicationCreateInfo renderCreateInfo);

        [[nodiscard]] NeoneuronRender& getRender();

        [[nodiscard]] const NeoneuronRender& getRender() const;

        [[nodiscard]] nlohmann::json& getSettings();

        [[nodiscard]] const nlohmann::json& getSettings() const;

        Storage<DefinedProperty>& getPropertyStorage();

        const Storage<DefinedProperty>& getPropertyStorage() const;

        Storage<LoaderBuilder>& getLoaderStorage();

        const Storage<LoaderBuilder>& getLoaderStorage() const;

        void registerSettingsListener(const hey::Listener<std::string>& listener) const;

        void unregisterSettingsListener(const hey::Listener<std::string>& listener) const;

        void signalSettingsChange(std::string node) const;

        void saveSettings() const;
    };
}


#endif //NEONEURONAPPLICATION_H
