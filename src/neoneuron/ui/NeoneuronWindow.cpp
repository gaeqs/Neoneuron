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
// Created by gaeqs on 13/11/25.
//

#include "NeoneuronWindow.h"

#include <imgui.h>

namespace neoneuron
{
    void NeoneuronWindow::setWindowName(const std::string& windowName)
    {
        _windowName = windowName;
    }

    void NeoneuronWindow::initWindow(bool* open)
    {
        float rounding = ImGui::GetStyle().WindowRounding;
        ImVec2 padding = ImGui::GetStyle().WindowPadding;
        ImU32 bgColor = ImGui::GetColorU32(ImGuiCol_WindowBg);

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImGui::GetStyleColorVec4(ImGuiCol_Border));
        if (ImGui::Begin(_windowName.c_str(), open)) {

            ImGui::PushStyleColor(ImGuiCol_FrameBg, bgColor);
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(padding.x, padding.y));
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, rounding);
            if (ImGui::BeginChild("##child", ImVec2(0, 0), ImGuiChildFlags_FrameStyle)) {
                ImGui::PopStyleVar(2);
                ImGui::PopStyleColor();
                drawWindow();
            } else {
                ImGui::PopStyleColor();
                ImGui::PopStyleVar(2);
            }
            ImGui::EndChild();
        }
        ImGui::End();
        ImGui::PopStyleColor();
        ImGui::PopStyleVar();
    }

    NeoneuronWindow::NeoneuronWindow(std::string windowName) :
        _windowName(std::move(windowName))
    {
    }

    void NeoneuronWindow::onPreDraw()
    {
        initWindow(nullptr);
    }
} // namespace neoneuron
