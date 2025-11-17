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

#include "NeoneuronUiSettings.h"

#include <imgui.h>
#include <neoneuron/application/NeoneuronApplication.h>
#include <neoneuron/ui/style/Fonts.h>
#include <neoneuron/ui/style/Themes.h>

namespace neoneuron::settings
{
    void settingsSection(const std::string& name)
    {
        ImGui::PushFont(nullptr, 32.0f);
        ImGui::Text(name.c_str());
        ImGui::PopFont();
    }

    void settingsTheme(NeoneuronApplication* app)
    {
        auto& files = app->getFiles();
        static int styleIdx = files.getSettings().value(NeoneuronFiles::SETTINGS_THEME, 0);
        static int current = styleIdx;
        ImGui::Combo("Theme", &styleIdx, "Dark Theme\0Light Theme\0");

        if (styleIdx != current) {
            current = styleIdx;
            files.getSettings()[NeoneuronFiles::SETTINGS_THEME] = styleIdx;
            files.signalSettingsChange(NeoneuronFiles::SETTINGS_THEME);

            switch (styleIdx) {
                case 0:
                    StyleColorsDark(app->getRender().getUI().getColorPalette());
                    break;
                case 1:
                    StyleColorsLight(app->getRender().getUI().getColorPalette());
                    break;
                default:
                    break;
            }
        }
    }

    void settingsFontSize(NeoneuronApplication* app)
    {
        auto& files = app->getFiles();

        static float scale = files.getSettings().value(NeoneuronFiles::SETTINGS_FONT_SCALE, 1.0f);

        if (ImGui::SliderFloat("Font scale", &scale, 0.5f, 5.0f, "%.2f")) {
            ImGui::GetStyle().FontScaleMain = scale;
            files.getSettings()[NeoneuronFiles::SETTINGS_FONT_SCALE] = scale;
            files.signalSettingsChange(NeoneuronFiles::SETTINGS_FONT_SCALE);
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
