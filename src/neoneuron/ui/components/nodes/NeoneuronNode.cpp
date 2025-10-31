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

//
// Created by gaeqs on 30/10/25.
//

#include "NeoneuronNode.h"

#include "imgui_internal.h"

#include <neoneuron/ui/style/MaterialSymbols.h>

namespace neoneuron
{
    NeoneuronNode::NeoneuronNode(std::string name, std::string icon, ImBlueprint::NodeTitleStyle style) :
        Node(std::move(name), std::move(style)),
        _icon(std::move(icon))
    {
    }

    void NeoneuronNode::renderTitle()
    {
        ImGui::PushFont(nullptr, 24.0f);

        auto pos = ImGui::GetCursorScreenPos();
        if (ImGui::InvisibleButton("node_close", ImVec2(ImGui::GetFontSize(), ImGui::GetFontSize()))) {
            askForDeletion();
        }
        ImGui::SetCursorScreenPos(pos);
        ImNodes::DynamicText(ImGui::IsItemHovered() ? ImGui::IsItemActive() ? ICON_MS_CLOSE_SMALL : ICON_MS_CLOSE
                                                    : _icon);

        ImGui::PopFont();
        ImGui::SameLine();
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4.0f);

        ImGui::PushFont(nullptr, 16.0f);
        Node::renderTitle();
        ImGui::PopFont();
    }
} // namespace neoneuron