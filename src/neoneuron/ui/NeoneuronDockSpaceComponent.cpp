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

#define IMGUI_DEFINE_MATH_OPERATORS

#include "NeoneuronDockSpaceComponent.h"

#include <imnodes.h>
#include <imnodes_draw.h>

#include <neoneuron/render/NeoneuronRender.h>

namespace neoneuron
{
    void NeoneuronDockSpaceComponent::renderTitleBackground(const ImGuiWindow* window)
    {
        auto drawList = window->DrawList;
        ImRect rect;

        if (window->Flags & ImGuiWindowFlags_NoTitleBar) {
            return;
        }

        if (window->DockIsActive) {
            auto* node = window->DockNode;

            while (node->ParentNode != nullptr) {
                node = node->ParentNode;
            }

            if (!node->IsDockSpace()) {
                window = node->HostWindow;
                if (window == nullptr) {
                    return;
                }
                drawList = window->DrawList;
                rect = window->Rect();
            } else {
                if (node->HostWindow == nullptr) {
                    return;
                }
                drawList = node->HostWindow->DrawList;
                rect = window->Rect();
            }
        } else {
            if (!window->Collapsed) {
                rect = window->Rect();
            } else {
                rect = window->TitleBarRect();
            }
        }
        ImDrawListSplitter splitter;
        splitter.Split(drawList, 2);
        splitter._Current = 1;                   // This forces the elements already drawn to be on the foreground
        splitter.SetCurrentChannel(drawList, 0); // Now we change to the background.

        // Now we can draw on the background!
        ImU32 color = _render->getUI().getColorPalette().surfaceContainer.normal.toImGuiUInt();

        drawList->PushClipRectFullScreen();
        drawList->AddRectFilled(rect.GetTL(), rect.GetBR(), color, ImGui::GetStyle().WindowRounding);

        // Add gradients!
        {
            auto gradientRect = rect;
            gradientRect.Max.y = std::min(rect.Min.y + 30.0f, rect.Max.y);

            ImU32 lighterColor = _render->getUI().getColorPalette().primary.darkest.toImGuiUInt();
            ImU32 lightColor = _render->getUI().getColorPalette().primaryAlt.darkest.toImGuiUInt();

            ImNodeRectColor gradient(lighterColor, lightColor, lighterColor, lightColor);

            ImDrawFlags flags =
                gradientRect.Max.y == rect.Max.y ? ImDrawFlags_RoundCornersAll : ImDrawFlags_RoundCornersTop;

            drawList->PathRect(gradientRect.Min, gradientRect.Max, ImGui::GetStyle().WindowRounding, flags);

            ImNodes::PathFillConvex(drawList, gradient, gradientRect.Min, gradientRect.Max);
        }
        if (rect.Max.y > rect.Min.y + 30.0f) {
            auto gradientRect = rect;
            gradientRect.Min.y += 30.0f;
            gradientRect.Max.y = std::min(gradientRect.Min.y + 50.0f, rect.Max.y);

            ImU32 lighterColor = _render->getUI().getColorPalette().primary.darkest.toImGuiUInt();
            ImU32 lightColor = _render->getUI().getColorPalette().primaryAlt.darkest.toImGuiUInt();

            ImNodeRectColor gradient(lighterColor, lightColor, color, color);

            drawList->PathRect(gradientRect.Min, gradientRect.Max, ImGui::GetStyle().WindowRounding,
                               ImDrawFlags_RoundCornersBottom);

            ImNodes::PathFillConvex(drawList, gradient, gradientRect.Min, gradientRect.Max);
        }

        drawList->PopClipRect();
        splitter.Merge(drawList);
    }

    void NeoneuronDockSpaceComponent::renderDockSpaceWindow(ImGuiViewport* vp, ImVec2 pos, ImVec2 size)
    {
        constexpr int DOCK_FLAGS = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBringToFrontOnFocus |
                                   ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_MenuBar;

        ImU32 background = _render->getUI().getColorPalette().surface.toImGuiUInt();
        ImU32 highlight = _render->getUI().getColorPalette().surfaceTint.toImGuiUInt();

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
        }
        ImGui::End();
        ImGui::PopStyleColor(3);
    }

    NeoneuronDockSpaceComponent::NeoneuronDockSpaceComponent(NeoneuronRender* render) :
        _render(render)
    {
    }

    void NeoneuronDockSpaceComponent::onLatePreDraw()
    {
        for (auto window : ImGui::GetCurrentContext()->Windows) {
            renderTitleBackground(window);
        }
    }
} // namespace neoneuron