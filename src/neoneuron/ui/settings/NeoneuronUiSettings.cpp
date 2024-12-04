//
// Created by gaeqs on 4/12/24.
//

#include "NeoneuronUiSettings.h"

#include <imgui.h>

namespace neoneuron::settings {
    void settingsDialog() {
        ImGui::BeginChild("settings_content", ImVec2(0, ImGui::GetContentRegionMax().y - 100.0f));

        for (size_t i = 0; i < 100; ++i) {
            ImGui::Text("Hello there! %d", i);
        }
        ImGui::EndChild();

        ImGui::Separator();
        ImGui::SetCursorPosY(ImGui::GetContentRegionMax().y - 40.0f);
        ImGui::SetCursorPosX(ImGui::GetContentRegionMax().x - 140.0f);
        if (ImGui::Button("Close", ImVec2(120, 0))) {
            ImGui::CloseCurrentPopup();
        }
    }
}
