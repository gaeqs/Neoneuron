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

#include "NeoneuronFiles.h"

#include <fstream>
#include <neon/util/FileUtils.h>

#include <neon/Neon.h>

CMRC_DECLARE(resources);

namespace neoneuron
{

    nlohmann::json NeoneuronFiles::loadSettings() const
    {
        neon::DirectoryFileSystem dfs(_configFolder);
        auto optional = dfs.readFile(CONFIG_FILE);
        if (!optional.has_value()) {
            return "{}"_json;
        }
        return optional->toJson().value_or(nlohmann::json());
    }

    NeoneuronFiles::NeoneuronFiles()
    {
        auto result = neon::createConfigDirectory(CONFIG_DIRECTORY);
        if (!result.isOk()) {
            neon::error() << "Cannot create config directory. Settings won't be saved.";
            return;
        }

        _configFolder = result.getResult();
        _settings = loadSettings();
    }

    NeoneuronFiles::~NeoneuronFiles()
    {
    }

    std::filesystem::path NeoneuronFiles::getConfigFolder() const
    {
        return _configFolder;
    }

    nlohmann::json& NeoneuronFiles::getSettings()
    {
        return _settings;
    }

    const nlohmann::json& NeoneuronFiles::getSettings() const
    {
        return _settings;
    }

    void NeoneuronFiles::registerSettingsListener(const hey::Listener<std::string>& listener) const
    {
        _settingsNodeChange.addListener(listener);
    }

    void NeoneuronFiles::unregisterSettingsListener(const hey::Listener<std::string>& listener) const
    {
        _settingsNodeChange.removeListener(listener);
    }

    void NeoneuronFiles::signalSettingsChange(std::string node) const
    {
        _settingsNodeChange.invoke(std::move(node));
    }

    void NeoneuronFiles::saveSettings() const
    {
        std::ofstream out(_configFolder / CONFIG_FILE);
        if (!out.good()) {
            return;
        }
        std::string data = _settings.dump(4);
        out.write(data.c_str(), data.size());
    }

    void NeoneuronFiles::loadImGuiIniFile() const
    {
        std::filesystem::path path = _configFolder / IMGUI_FILE;
        bool exists = std::filesystem::exists(path);
        bool isDirectory = std::filesystem::is_directory(path);
        if (!exists || isDirectory) {
            neon::CMRCFileSystem fs(cmrc::resources::get_filesystem());
            auto file = fs.readFile("imgui.ini");
            if (!file.has_value()) {
                neon::error() << "Cannot find embedded imgui.ini!";
                return;
            }

            neon::info() << "Load imgui settings from memory.";
            ImGui::LoadIniSettingsFromMemory(file->getData(), file->getSize());
            return;
        }

        neon::info() << "Load imgui settings from " << path;
        ImGui::LoadIniSettingsFromDisk(path.string().c_str());
    }

    void NeoneuronFiles::saveImGuiIniFile() const
    {
        std::filesystem::path path = _configFolder / IMGUI_FILE;
        if (std::filesystem::is_directory(path)) {
            neon::warning() << "ImGui init file (" << path << ") is a directory. Cannot save ini file.";
            return;
        }
        ImGui::SaveIniSettingsToDisk(path.string().c_str());
    }

} // namespace neoneuron