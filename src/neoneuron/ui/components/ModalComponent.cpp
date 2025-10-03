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

#include "ModalComponent.h"

#include "neon/util/ImGuiUtils.h"

#include <imgui.h>

namespace neoneuron
{
    ModalComponent::ModalComponent(std::string name, bool hasActionButton) :
        _name(std::move(name)),
        _hasActionButton(hasActionButton),
        _open(false)
    {
    }

    void ModalComponent::onPreDraw()
    {
        if (!_open) {
            ImGui::OpenPopup(_name.c_str());
            _open = true;
        }

        ImGui::SetNextWindowSizeConstraints(ImVec2(400, 150), ImVec2(1000, 1000));
        if (ImGui::BeginPopupModal(_name.c_str(), &_open)) {
            auto& layout = ImGui::neon::BeginColumnLayout("modal_layout");

            if (ImGui::BeginChild("content", ImVec2(0.0f, layout.popStretchedSize()),
                                  ImGuiChildFlags_AlwaysUseWindowPadding)) {
                layout.next(true, 1.0f);
                onModalDraw();
            }
            ImGui::EndChild();
            if (_hasActionButton) {
                ImGui::Separator();
                layout.next();

                layout.space(ImGui::GetFrameHeight() / 2.0f);

                auto& buttonsLayout = ImGui::neon::BeginRowLayout("modal_layout_buttons");
                buttonsLayout.stretch();
                actionButton(buttonsLayout);
                buttonsLayout.end();
                layout.next();

                layout.space(ImGui::GetFrameHeight() / 2.0f);
            }

            layout.end();
            ImGui::EndPopup();
        }

        if (!_open) {
            getGameObject()->destroy();
        }
    }
} // namespace neoneuron
