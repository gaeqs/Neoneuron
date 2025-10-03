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

#include "rush/vector/vec.h"

#include <imgui_impl_vulkan.h>
#include <unordered_map>

namespace
{

    struct MergedFont
    {
        std::string name;
        std::unique_ptr<std::array<ImWchar, 3>> range;
        neon::File file;
        rush::Vec2f offset;
    };

    std::unordered_map<std::string, ImFont*> _fonts;
    std::vector<MergedFont> _mergedFonts;
} // namespace

namespace neoneuron::fonts
{
    bool loadFont(const std::string& name, const neon::File& file)
    {
        if (!file.isValid()) {
            return false;
        }
        auto& io = ImGui::GetIO();
        ImFontConfig font_cfg;
        font_cfg.FontDataOwnedByAtlas = false;
        font_cfg.OversampleH = 3.;
        font_cfg.OversampleV = 1.;
        font_cfg.RasterizerMultiply = 1.25f;

        std::memcpy(font_cfg.Name, name.data(), std::min(static_cast<size_t>(40), name.size()));

        void* data = const_cast<std::byte*>(file.getData());
        auto* font = io.Fonts->AddFontFromMemoryTTF(data, static_cast<int>(file.getSize()), 18.0f, &font_cfg);
        _fonts[name] = font;

        font_cfg.MergeMode = true;

        for (auto& merged : _mergedFonts) {
            std::string finalName = name + " - " + merged.name;
            void* mergedData = const_cast<std::byte*>(merged.file.getData());

            std::memcpy(font_cfg.Name, finalName.data(), std::min(static_cast<size_t>(40), finalName.size()));
            font_cfg.GlyphOffset = ImVec2(merged.offset.x(), merged.offset.y());

            io.Fonts->AddFontFromMemoryTTF(mergedData, static_cast<int>(merged.file.getSize()), 18.0f, &font_cfg,
                                           merged.range->data());
        }

        return true;
    }

    bool addMergeFont(const std::string& name, neon::File&& file, neon::Range<ImWchar> loadRange, rush::Vec2f offset)
    {
        if (!file.isValid()) {
            return false;
        }
        MergedFont mergedFont;
        mergedFont.name = name;
        mergedFont.range =
            std::make_unique<std::array<ImWchar, 3>>(std::array<ImWchar, 3>{loadRange.getFrom(), loadRange.getTo(), 0});
        mergedFont.file = std::move(file);
        mergedFont.offset = offset;
        _mergedFonts.push_back(std::move(mergedFont));
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
} // namespace neoneuron::fonts
