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

//
// Created by gaeqs on 08/05/2025.
//

#ifndef NEONEURONFILES_H
#define NEONEURONFILES_H

#include <hey/Hey.h>
#include <nlohmann/json.hpp>

namespace neoneuron
{
    class NeoneuronFiles
    {
      public:
        inline static const std::string CONFIG_DIRECTORY = "Neoneuron";
        inline static const std::string CONFIG_FILE = "neoneuron.json";
        inline static const std::string IMGUI_FILE = "imgui.ini";
        static inline const std::string SETTINGS_THEME = "theme";
        static inline const std::string SETTINGS_FONT_SIZE = "font_size";

        static inline const std::string SETTINGS_TOOL_GLOBAL_PARAMETERS = "tool_global_parameters";
        static inline const std::string SETTINGS_TOOL_DEBUG = "tool_debug";
        static inline const std::string SETTINGS_TOOL_DEMO = "tool_demo";

        nlohmann::json loadSettings() const;

      private:
        std::filesystem::path _configFolder;
        nlohmann::json _settings;
        hey::Observable<std::string> _settingsNodeChange;

      public:
        NeoneuronFiles();

        ~NeoneuronFiles();

        /**
         * Retrieves the path to the configuration folder.
         *
         * @return The path to the configuration folder.
         */
        [[nodiscard]] std::filesystem::path getConfigFolder() const;

        /**
         * A JSON file used to store the global settings of Neoneuron.
         */
        [[nodiscard]] nlohmann::json& getSettings();

        /**
         * A JSON file used to store the global settings of Neoneuron.
         */
        [[nodiscard]] const nlohmann::json& getSettings() const;

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
         * Signals a parameter change inside the settings' JSON.
         */
        void signalSettingsChange(std::string node) const;

        /**
         * Saves the settings' JSON into a file.
         */
        void saveSettings() const;

        void loadImGuiIniFile() const;

        void saveImGuiIniFile() const;
    };
} // namespace neoneuron

#endif // NEONEURONFILES_H
