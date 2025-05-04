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

#include "Fonts.h"

#include <imgui_impl_vulkan.h>
#include <unordered_map>

namespace
{
    std::unordered_map<std::string, ImFont*> _fonts;
}

namespace neoneuron::fonts
{
    bool loadFont(const std::string& name, const neon::File& file, float sizeInPixels)
    {
        if (!file.isValid()) {
            return false;
        }
        auto& io = ImGui::GetIO();
        ImFontConfig font_cfg;
        font_cfg.FontDataOwnedByAtlas = false;

        std::memcpy(font_cfg.Name, name.data(), std::min(static_cast<size_t>(40), name.size()));

        void* data = const_cast<char*>(file.getData());
        auto* font = io.Fonts->AddFontFromMemoryTTF(data, file.getSize(), sizeInPixels, &font_cfg);
        _fonts[name] = font;
        return true;
    }

    std::optional<ImFont*> getFont(const std::string& name)
    {
        auto it = _fonts.find(name);
        if (it == _fonts.end()) {
            return {};
        }
        return it->second;
    }

    void recreateFonts()
    {
        ImGui_ImplVulkan_CreateFontsTexture();
    }

    void imGuiPushFont(const std::string& name)
    {
        if (auto it = _fonts.find(name); it == _fonts.end()) {
            ImGui::PushFont(nullptr);
        } else {
            ImGui::PushFont(it->second);
        }
    }
} // namespace neoneuron::fonts
