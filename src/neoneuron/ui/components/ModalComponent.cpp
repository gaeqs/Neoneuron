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
} // namespace neoneuron
