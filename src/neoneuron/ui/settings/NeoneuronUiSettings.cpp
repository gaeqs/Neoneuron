//
// Created by gaeqs on 4/12/24.
//

#include "NeoneuronUiSettings.h"

#include <imgui.h>
#include <neoneuron/application/NeoneuronApplication.h>
#include <neoneuron/ui/style/Fonts.h>
#include <neoneuron/ui/style/Themes.h>

namespace neoneuron::settings {
    void settingsSection(const std::string& name) {
        auto font = fonts::getFont(fonts::SS3_32);

        if (font.has_value()) ImGui::PushFont(font.value());
        ImGui::Text(name.c_str());
        if (font.has_value()) ImGui::PopFont();
    }

    void settingsTheme(NeoneuronApplication* app) {
        static int styleIdx = app->getSettings().value(NeoneuronApplication::SETTINGS_THEME, 0);
        static int current = styleIdx;
        ImGui::Combo("Theme", &styleIdx, "Dark Theme\0Light Theme\0");
        switch (styleIdx) {
            case 0:
                StyleColorsDark();
                break;
            case 1:
                StyleColorsLight();
                break;
            default:
                break;
        }

        if (styleIdx != current) {
            current = styleIdx;
            app->getSettings()[NeoneuronApplication::SETTINGS_THEME] = styleIdx;
            app->signalSettingsChange(NeoneuronApplication::SETTINGS_THEME);
        }
    }

    void settingsDialog(NeoneuronApplication* app) {
        ImGui::BeginChild("settings_content", ImVec2(0, ImGui::GetContentRegionMax().y - 100.0f));

        ImGui::SetCursorPosX(20.0f);
        settingsSection("Display");

        ImGui::SetCursorPosX(30.0f);
        settingsTheme(app);

        ImGui::EndChild();

        ImGui::Separator();
        ImGui::SetCursorPosY(ImGui::GetContentRegionMax().y - 40.0f);
        ImGui::SetCursorPosX(ImGui::GetContentRegionMax().x - 140.0f);
        if (ImGui::Button("Close", ImVec2(120, 0))) {
            ImGui::CloseCurrentPopup();
        }
    }
}
