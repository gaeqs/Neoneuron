//
// Created by gaeqs on 4/12/24.
//

#ifndef NEONEURONAPPLICATION_H
#define NEONEURONAPPLICATION_H

#include <mindset/Dataset.h>
#include <mindset/loader/LoaderRegistry.h>
#include <neoneuron/render/NeoneuronRender.h>
#include <neoneuron/structure/Selector.h>
#include <neoneuron/structure/Storage.h>
#include <neoneuron/structure/property/DefinedProperty.h>

namespace neoneuron
{

    /**
    * This is the main class of Neoneuron.
    * The application should only have one instance of this class.
    */
    class NeoneuronApplication
    {
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

        mindset::Dataset _dataset;
        Selector _selector;
        NeoneuronRender _render;
        Storage<DefinedProperty> _propertyStorage;
        mindset::LoaderRegistry _loaderStorage;

        static nlohmann::json loadSettings();

      public:
        NeoneuronApplication(const NeoneuronApplication&) = delete;

        explicit NeoneuronApplication(neon::vulkan::VKApplicationCreateInfo renderCreateInfo);

        /**
        * The dataset where all the neuronal information is stored.
        * Insert your brain data here!
        */
        [[nodiscard]] mindset::Dataset& getDataset();

        /**
        * The dataset where all the neuronal information is stored.
        * Insert your brain data here!
        */
        [[nodiscard]] const mindset::Dataset& getDataset() const;

        /**
        * The selection of the application.
        */
        [[nodiscard]] Selector& getSelector();

        /**
        * The selection of the application.
        */
        [[nodiscard]] const Selector& getSelector() const;

        /**
        * This class manages the render part of the application.
        */
        [[nodiscard]] NeoneuronRender& getRender();

        /**
        * This class manages the render part of the application.
        */
        [[nodiscard]] const NeoneuronRender& getRender() const;

        /**
        * A JSON file used to store the global settings of Neoneuron.
        */
        [[nodiscard]] nlohmann::json& getSettings();

        /**
        * A JSON file used to store the global settings of Neoneuron.
        */
        [[nodiscard]] const nlohmann::json& getSettings() const;

        /**
        * Storage used to store all properties' definitions.
        * If you want to define a new property that can be saved, loaded and edited,
        * you should store its definition here!
        */
        Storage<DefinedProperty>& getPropertyStorage();

        /**
        * Storage used to store all properties' definitions.
        * If you want to define a new property that can be saved, loaded and edited,
        * you should store its definition here!
        */
        const Storage<DefinedProperty>& getPropertyStorage() const;

        /**
        * This registry is used to register loaders Neoneuron can use to load scenes.
        * If you have created a new loader, you should register it here!
        */
        mindset::LoaderRegistry& getLoaderRegistry();

        /**
        * This registry is used to register loaders Neoneuron can use to load scenes.
        * If you have created a new loader, you should register it here!
        */
        const mindset::LoaderRegistry& getLoaderRegistry() const;

        /**
         * Registers a new listener that will be called when a parameter inside the settings' json is changed.
         * The listener will only receive the key of the changed parameter.
         */
        void registerSettingsListener(const hey::Listener<std::string>& listener) const;

        /**
        * Unregisters a listener registered using "registerSettingsListener()".
        */
        void unregisterSettingsListener(const hey::Listener<std::string>& listener) const;

        /**
        * Signals a parameter change inside the settings' json.
        */
        void signalSettingsChange(std::string node) const;

        /**
        * Saves the settings' json into a file.
        */
        void saveSettings() const;
    };
} // namespace neoneuron

#endif //NEONEURONAPPLICATION_H
