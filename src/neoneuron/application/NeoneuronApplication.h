// Copyright (c) 2025. VG-Lab/URJC.
//
// Authors: Gael Rial Costas <gael.rial.costas@urjc.es>
//
// This file is part of Neoneuron <gitlab.gmrv.es/g.rial/neoneuron>
//
// This library is free software; you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License version 3.0 as published
// by the Free Software Foundation.
//
// This library is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this library; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

#ifndef NEONEURONAPPLICATION_H
#define NEONEURONAPPLICATION_H

#include "neoneuron/loader/LoaderCollection.h"

#include <mindset/loader/LoaderRegistry.h>
#include <neoneuron/render/NeoneuronRender.h>
#include <neoneuron/structure/Repository.h>
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

        Repository _repository;
        Selector _selector;
        NeoneuronRender _render;
        Storage<DefinedProperty> _propertyStorage;
        mindset::LoaderRegistry _loaderStorage;
        LoaderCollection _loaderCollection;

        static nlohmann::json loadSettings();

      public:
        NeoneuronApplication(const NeoneuronApplication&) = delete;

        explicit NeoneuronApplication(const neon::vulkan::VKApplicationCreateInfo& renderCreateInfo);

        /**
         * The repository where all the neuronal information is stored.
         * Insert your brain data here!
         */
        [[nodiscard]] Repository& getRepository();

        /**
         * The repository where all the neuronal information is stored.
         * Insert your brain data here!
         */
        [[nodiscard]] const Repository& getRepository() const;

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
         * This collection is used to manage and storage running loaders.
         * Using this collection, you can retrieve the status of the
         * currently loading datasets.
         */
        LoaderCollection& getLoaderCollection();

        /**
         * This collection is used to manage and storage running loaders.
         * Using this collection, you can retrieve the status of the
         * currently loading datasets.
         */
        const LoaderCollection& getLoaderCollection() const;

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

#endif // NEONEURONAPPLICATION_H
