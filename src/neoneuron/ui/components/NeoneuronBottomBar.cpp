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

#include "NeoneuronBottomBar.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "neoneuron/application/NeoneuronApplication.h"

namespace neoneuron
{

    void NeoneuronBottomBar::loadingStatus() const
    {
        float height = ImGui::GetFrameHeight() * 0.7f;

        auto& loaders = _application->getLoaderCollection();
        if (loaders.getLoadingAmount() > 0) {
            ImGui::ProgressBar(-1.0f * static_cast<float>(ImGui::GetTime()), ImVec2(500.0f, height),
                               "Loading dataset...");
        }
    }

    NeoneuronBottomBar::NeoneuronBottomBar(NeoneuronApplication* application) :
        _application(application)
    {
    }

    void NeoneuronBottomBar::onPreDraw()
    {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGuiWindowFlags window_flags =
            ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar;
        float height = ImGui::GetFrameHeight();

        if (ImGui::BeginViewportSideBar("##Status", viewport, ImGuiDir_Down, height, window_flags)) {
            if (ImGui::BeginMenuBar()) {
                ImGui::SetCursorPosY(height * 0.15f);
                loadingStatus();
                ImGui::EndMenuBar();
            }
            ImGui::End();
        }
    }
} // namespace neoneuron