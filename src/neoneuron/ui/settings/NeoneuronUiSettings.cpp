//
// Created by gaeqs on 4/12/24.
//

#include "NeoneuronUiSettings.h"

#include <imgui.h>
#include <neoneuron/application/NeoneuronApplication.h>
#include <neoneuron/ui/style/Fonts.h>
#include <neoneuron/ui/style/Themes.h>

namespace neoneuron::settings
{
    void settingsSection(const std::string& name)
    {
        auto font = fonts::getFont(fonts::SS3_32);

        if (font.has_value()) {
            ImGui::PushFont(font.value());
        }
        ImGui::Text(name.c_str());
        if (font.has_value()) {
            ImGui::PopFont();
        }
    }

    void settingsTheme(NeoneuronApplication* app)
    {
        static int styleIdx = app->getSettings().value(NeoneuronApplication::SETTINGS_THEME, 0);
        static int current = styleIdx;
        ImGui::Combo("Theme", &styleIdx, "Dark Theme\0Light Theme\0");

        if (styleIdx != current) {
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

            current = styleIdx;
            app->getSettings()[NeoneuronApplication::SETTINGS_THEME] = styleIdx;
            app->signalSettingsChange(NeoneuronApplication::SETTINGS_THEME);
        }
    }

    void settingsFontSize(NeoneuronApplication* app)
    {
        static int fontIdx = app->getSettings().value(NeoneuronApplication::SETTINGS_FONT_SIZE, 1);
        static int current = fontIdx;
        ImGui::Combo("Font size", &fontIdx, "Small\0Normal\0Large\0Extra large\0Giant\0");

        if (fontIdx != current) {
            const char* font = fonts::SS3_18;
            switch (fontIdx) {
                case 0:
                    font = fonts::SS3_16;
                    break;
                case 1:
                    font = fonts::SS3_18;
                    break;
                case 2:
                    font = fonts::SS3_20;
                    break;
                case 3:
                    font = fonts::SS3_24;
                    break;
                case 4:
                    font = fonts::SS3_32;
                    break;
                default:
                    break;
            }
            if (auto opt = fonts::getFont(font); opt.has_value()) {
                ImGui::GetIO().FontDefault = opt.value();
            }

            current = fontIdx;
            app->getSettings()[NeoneuronApplication::SETTINGS_FONT_SIZE] = fontIdx;
            app->signalSettingsChange(NeoneuronApplication::SETTINGS_FONT_SIZE);
        }
    }

    void settingsDialog(NeoneuronApplication* app)
    {
        ImGui::BeginChild("settings_content", ImVec2(0, ImGui::GetContentRegionMax().y - 100.0f));

        ImGui::Indent();
        settingsSection("Display");
        ImGui::Indent();
        settingsTheme(app);
        settingsFontSize(app);

        ImGui::Unindent();
        ImGui::Unindent();

        ImGui::EndChild();

        ImGui::Separator();
        ImGui::SetCursorPosY(ImGui::GetContentRegionMax().y - 40.0f);
        ImGui::SetCursorPosX(ImGui::GetContentRegionMax().x - 140.0f);
        if (ImGui::Button("Close", ImVec2(120, 0))) {
            ImGui::CloseCurrentPopup();
        }
    }
} // namespace neoneuron::settings
