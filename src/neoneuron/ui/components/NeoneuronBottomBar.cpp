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
#include "neoneuron/version.h"
#include "neoneuron/application/NeoneuronApplication.h"

namespace neoneuron
{

    void NeoneuronBottomBar::multiLoaderStatusBar() const
    {
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - LOADING_STATUS_WIDTH);

        auto text = std::format("Loading {} datasets...", _application->getLoaderCollection().getLoadingAmount());
        ImGui::ProgressBar(-1.0f * static_cast<float>(ImGui::GetTime()), ImVec2(LOADING_STATUS_WIDTH, 0.0f),
                           text.c_str());
    }

    void NeoneuronBottomBar::singleLoaderStatusBar() const
    {
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - LOADING_STATUS_WIDTH);

        auto& loaders = _application->getLoaderCollection();
        auto& status = (*loaders.getLoaders().begin())->latestStatus;
        std::string text = "Loading unknown data...";
        float fraction = -1.0f * static_cast<float>(ImGui::GetTime());
        if (status != nullptr) {
            text = std::format("Loading dataset: {} ({}/{})", status->currentTask, status->stagesCompleted,
                               status->stages);
            fraction = static_cast<float>(status->stagesCompleted) / status->stages;
        }

        ImGui::ProgressBar(fraction, ImVec2(LOADING_STATUS_WIDTH, 0.0f), text.c_str());
    }

    void NeoneuronBottomBar::loadingStatus() const
    {
        auto& loaders = _application->getLoaderCollection();
        switch (loaders.getLoadingAmount()) {
            case 0:
                return;
            case 1:
                singleLoaderStatusBar();
                return;
            default:
                multiLoaderStatusBar();
        }
    }

    NeoneuronBottomBar::NeoneuronBottomBar(NeoneuronApplication* application,
                                           neon::IdentifiableWrapper<neon::DockSpaceComponent> dockSpace) :
        _application(application),
        _dockSpace(std::move(dockSpace))
    {
        _sidebar = _dockSpace->addSidebar(neon::DockSidebarPosition::BOTTOM, imGuiUId("BottomBar"), 24.0f);
    }

    NeoneuronBottomBar::~NeoneuronBottomBar()
    {
        if (_dockSpace) {
            _dockSpace->removeSidebar(_sidebar);
        }
    }

    void NeoneuronBottomBar::onPreDraw()
    {
        _sidebar->size = ImGui::GetFrameHeight() + ImGui::GetStyle().WindowPadding.y * 2.0f;
        constexpr ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking |
                                           ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings |
                                           ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImGui::GetStyle().Colors[ImGuiCol_DockingEmptyBg]);
        if (ImGui::Begin(imGuiUId("BottomBar").c_str(), nullptr, flags)) {
            float fps = 1.0f / getApplication()->getCurrentFrameInformation().lastFrameProcessTime;

            ImGui::Text("Neoneuron " NEONEURON_VERSION " (" NEONEURON_GIT_COMMIT ")");
            ImGui::SameLine();
            ImGui::Text("%.2f FPS", fps);
            ImGui::SameLine();
            loadingStatus();
            ImGui::End();
        }
        ImGui::PopStyleVar(1);
        ImGui::PopStyleColor(1);
    }
} // namespace neoneuron