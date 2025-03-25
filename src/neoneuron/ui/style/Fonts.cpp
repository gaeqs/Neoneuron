//
// Created by gaeqs on 11/11/2024.
//

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
