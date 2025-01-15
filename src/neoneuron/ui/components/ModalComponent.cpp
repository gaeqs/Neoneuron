//
// Created by gaeqs on 15/01/2025.
//

#include "ModalComponent.h"

#include <imgui.h>

namespace neoneuron {
    ModalComponent::ModalComponent(std::string name, bool hasActionButton)
        : _name(std::move(name)), _hasActionButton(hasActionButton), _open(false) {}

    void ModalComponent::onPreDraw() {
        if (!_open) {
            ImGui::OpenPopup(_name.c_str());
            _open = true;
        }

        ImGui::SetNextWindowSizeConstraints(ImVec2(400, 150), ImVec2(1000, 1000));
        if (ImGui::BeginPopupModal(_name.c_str(), &_open)) {
            onModalDraw();
            if (_hasActionButton) {
                ImGui::Separator();
                ImGui::SetCursorPosY(ImGui::GetContentRegionMax().y - 40.0f);
                ImGui::SetCursorPosX(ImGui::GetContentRegionMax().x - 140.0f);
                actionButton(ImVec2(120.0f, 0.0f));
            }
            ImGui::EndPopup();
        }

        if (!_open) {
            getGameObject()->destroy();
        }
    }
}
