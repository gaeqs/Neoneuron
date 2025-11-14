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
// Created by gaeqs on 14/11/25.
//

#include "NeoneuronDockSpaceComponent.h"

#include "neoneuron/render/NeoneuronRender.h"

#include <imgui_internal.h>

namespace neoneuron
{
    void NeoneuronDockSpaceComponent::renderDockSpaceWindow(ImGuiViewport* vp, ImVec2 pos, ImVec2 size)
    {
        constexpr int DOCK_FLAGS = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBringToFrontOnFocus |
                                   ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_MenuBar;

        ImU32 background = ImGui::ColorConvertFloat4ToU32(_render->getUI().getColorPalette().surface.Value);
        ImU32 highlight = ImGui::ColorConvertFloat4ToU32(_render->getUI().getColorPalette().surfaceTint.Value);
        ImU32 color = ImGui::GetColorU32(ImGuiCol_WindowBg);

        ImGui::SetNextWindowPos(vp->Pos);
        ImGui::SetNextWindowSize(vp->Size);
        ImDrawList* draw = ImGui::GetBackgroundDrawList(vp);
        ImVec2 max = ImVec2(vp->Pos.x + vp->Size.x, vp->Pos.y + vp->Size.y);
        ImVec2 max2 = ImVec2(vp->Pos.x + vp->Size.x / 2, vp->Pos.y + vp->Size.y / 2);
        draw->AddRectFilled(vp->Pos, max, background);
        draw->AddRectFilledMultiColor(vp->Pos, max2, highlight, background, background, background);

        ImGui::SetNextWindowPos(pos);
        ImGui::SetNextWindowSize(size);
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0));
        ImGui::PushStyleColor(ImGuiCol_DockingEmptyBg, ImVec4(0, 0, 0, 0));
        ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4(0, 0, 0, 0));
        if (ImGui::Begin("##DockingHost", nullptr, DOCK_FLAGS)) {
            ImGuiID dockSpaceId = ImGui::GetID("HUB_DockSpace");
            ImGui::DockSpace(dockSpaceId, ImVec2(0, 0), ImGuiDockNodeFlags_None);

            for (auto& data : GImGui->DockContext.Nodes.Data) {
                auto* dockNode = static_cast<ImGuiDockNode*>(data.val_p);
                if (dockNode == nullptr) {
                    continue;
                }
                auto* tabBar = dockNode->TabBar;
                if (tabBar != nullptr) {
                    auto rect = tabBar->BarRect;
                    rect.Min.x = dockNode->Pos.x;
                    rect.Max.x = dockNode->Pos.x + dockNode->Size.x;

                    // Fills the gap left by the rounding of the own window.
                    rect.Max.y += ImGui::GetStyle().WindowRounding;

                    draw->AddRectFilled(rect.GetTL(), rect.GetBR(), color, 4.0f,
                                        ImDrawFlags_RoundCornersTopLeft | ImDrawFlags_RoundCornersTopRight);
                }
            }
        }
        ImGui::End();
        ImGui::PopStyleColor(3);
    }

    NeoneuronDockSpaceComponent::NeoneuronDockSpaceComponent(NeoneuronRender* render) :
        _render(render)
    {
    }
} // namespace neoneuron