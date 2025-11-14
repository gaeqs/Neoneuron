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

#include "NeoneuronTopBar.h"

#include "../../../cmake-build-release/_deps/neon-src/lib/imgui/imgui_internal.h"

#ifdef WIN32
    #define GLFW_EXPOSE_NATIVE_WIN32
#endif

#include <neoneuron/loader/SceneLoader.h>

#include <neoneuron/render/NeoneuronRender.h>
#include <neoneuron/render/complex/ComplexNeuronRepresentation.h>

#include "components/NeoneuronUIOpenFile.h"
#include "components/actions/ActionSave.h"
#include "components/actions/ActionShuffle.h"
#include "components/NeoneuronUIAbout.h"
#include "components/actions/ActionDuplicate.h"
#include "neon/util/dialog/Dialogs.h"
#include "settings/NeoneuronUiSettings.h"
#include "style/Fonts.h"
#include "style/MaterialSymbols.h"

namespace neoneuron
{

    void NeoneuronTopBar::saveFile(const std::string& data) const
    {
        neon::SaveFileDialogInfo info;
        info.application = getApplication();
        info.defaultExtension = "json";
        if (auto result = neon::saveFileDialog(info)) {
            if (std::ofstream out(result.value().string()); out) {
                out << data;
            }
        }
    }

    void NeoneuronTopBar::toolsMenu() const
    {
        static const auto NEURON_LIST = NeoneuronFiles::SETTINGS_TOOL_NEURON_LIST;
        static const auto GLOBAL_PARAMS = NeoneuronFiles::SETTINGS_TOOL_GLOBAL_PARAMETERS;
        static const auto DEBUG = NeoneuronFiles::SETTINGS_TOOL_DEBUG;
        static const auto PERFORMANCE = NeoneuronFiles::SETTINGS_TOOL_PERFORMANCE;
        static const auto DEMO = NeoneuronFiles::SETTINGS_TOOL_DEMO;
        auto& files = _render->getNeoneuronApplication()->getFiles();
        auto& s = files.getSettings();

        bool neuronList = s.value(NEURON_LIST, false);
        bool globalParameters = s.value(GLOBAL_PARAMS, false);
        bool debug = s.value(DEBUG, false);
        bool performance = s.value(PERFORMANCE, false);
        bool demo = s.value(DEMO, false);

        if (ImGui::MenuItem(ICON_MS_NEUROLOGY "Neurons", nullptr, &neuronList)) {
            s[NEURON_LIST] = neuronList;
            files.signalSettingsChange(NEURON_LIST);
        }

        if (ImGui::MenuItem(ICON_MS_GLOBE "Global parameters", nullptr, &globalParameters)) {
            s[GLOBAL_PARAMS] = globalParameters;
            files.signalSettingsChange(GLOBAL_PARAMS);
        }

        if (ImGui::MenuItem(ICON_MS_INFO "Debug", nullptr, &debug)) {
            s[DEBUG] = debug;
            files.signalSettingsChange(DEBUG);
        }

        if (ImGui::MenuItem(ICON_MS_PERFORMANCE_MAX "Performance Recorder", nullptr, &performance)) {
            s[PERFORMANCE] = performance;
            files.signalSettingsChange(PERFORMANCE);
        }

        if (ImGui::MenuItem(ICON_MS_IMAGESEARCH_ROLLER "ImGUI Demo", nullptr, &demo)) {
            s[DEMO] = demo;
            files.signalSettingsChange(DEMO);
        }
    }

    void NeoneuronTopBar::viewMenu() const
    {
        // auto* scene = _render->getNeuronScene().get();
        // if (auto* complexScene = dynamic_cast<ComplexNeuronRepresentation*>(scene)) {
        //     bool drawSegments = complexScene->shouldDrawSegments();
        //     bool drawJoints = complexScene->shouldDrawJoints();
        //     bool drawSomas = complexScene->shouldDrawSomas();
        //     if (ImGui::MenuItem("Draw segments", nullptr, &drawSegments)) {
        //         complexScene->setDrawSegments(drawSegments);
        //     }
        //     if (ImGui::MenuItem("Draw joints", nullptr, &drawJoints)) {
        //         complexScene->setDrawJoints(drawJoints);
        //     }
        //     if (ImGui::MenuItem("Draw somas", nullptr, &drawSomas)) {
        //         complexScene->setDrawSomas(drawSomas);
        //     }
        // }
    }

    void NeoneuronTopBar::actionsMenu() const
    {
        auto app = _render->getNeoneuronApplication();
        if (ImGui::MenuItem(ICON_MS_RECENTER "Focus scene")) {
            _render->focusScene();
        }
        if (ImGui::MenuItem(ICON_MS_SHUFFLE "Shuffle")) {
            _render->getRoom()->newGameObject()->newComponent<ActionShuffle>(app);
        }
        if (ImGui::MenuItem(ICON_MS_TAB_DUPLICATE "Duplicate")) {
            _render->getRoom()->newGameObject()->newComponent<ActionDuplicate>(app);
        }
        if (ImGui::MenuItem(ICON_MS_DOWNLOAD "Save neuron model")) {
            _render->getRoom()->newGameObject()->newComponent<ActionSave>(app);
        }
    }

    void NeoneuronTopBar::demo() const
    {
        auto& files = _render->getNeoneuronApplication()->getFiles();
        bool opened = files.getSettings().value(NeoneuronFiles::SETTINGS_TOOL_DEMO, false);
        bool keepOpen = true;
        if (opened) {
            ImGui::ShowDemoWindow(&keepOpen);
        }
        if (opened && !keepOpen) {
            files.getSettings()[NeoneuronFiles::SETTINGS_TOOL_DEMO] = false;
            files.signalSettingsChange(NeoneuronFiles::SETTINGS_TOOL_DEMO);
        }
    }

    NeoneuronTopBar::NeoneuronTopBar(NeoneuronRender* render,
                                     neon::IdentifiableWrapper<neon::DockSpaceComponent> dockSpace) :
        _render(render),
        _dockSpace(dockSpace)
    {
        _sidebar = _dockSpace->addSidebar(neon::DockSidebarPosition::TOP, imGuiUId("TopBar"), 32.0f);
    }

    NeoneuronTopBar::~NeoneuronTopBar()
    {
        if (_dockSpace) {
            _dockSpace->removeSidebar(_sidebar);
        }
    }

    void NeoneuronTopBar::onStart()
    {
    }

    void NeoneuronTopBar::onPreDraw()
    {
        constexpr ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking |
                                           ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings |
                                           ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
                                           ImGuiWindowFlags_MenuBar;

        bool openSettings = false;
        ImGui::PushFont(nullptr, 20.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4(0, 0, 0, 0));
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0));
        if (ImGui::Begin(imGuiUId("TopBar").c_str(), nullptr, flags)) {
            if (ImGui::BeginMenuBar()) {
                if (ImGui::BeginMenu(ICON_MS_FILES "File")) {
                    ImGui::PushFont(nullptr, 18.0f);
                    if (ImGui::MenuItem(ICON_MS_FILE_OPEN "Open file", "Ctrl+O")) {
                        NeoneuronUiOpenFile::openDialog(_render->getNeoneuronApplication());
                    }
                    if (ImGui::MenuItem(ICON_MS_CLOSE "Close scene")) {
                        _render->getNeoneuronApplication()->getRepository().clear();
                    }
                    if (ImGui::MenuItem(ICON_MS_SAVE "Save scene")) {
                        saveFile(saveScene(_render).dump(4));
                    }
                    if (ImGui::MenuItem(ICON_MS_SETTINGS "Settings", "Ctrl+S")) {
                        openSettings = true;
                    }
                    ImGui::PopFont();
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu(ICON_MS_VISIBILITY "View")) {
                    ImGui::PushFont(nullptr, 18.0f);
                    viewMenu();
                    ImGui::PopFont();
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu(ICON_MS_BUILD "Tools")) {
                    ImGui::PushFont(nullptr, 18.0f);
                    toolsMenu();
                    ImGui::PopFont();
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu(ICON_MS_PLAY_ARROW "Actions")) {
                    ImGui::PushFont(nullptr, 18.0f);
                    actionsMenu();
                    ImGui::PopFont();
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu(ICON_MS_HELP "Help")) {
                    ImGui::PushFont(nullptr, 18.0f);
                    if (ImGui::MenuItem(ICON_MS_INFO "About")) {
                        _render->getRoom()->newGameObject()->newComponent<NeoneuronUIAbout>(
                            _render->getNeoneuronApplication());
                    }
                    ImGui::PopFont();
                    ImGui::EndMenu();
                }
                ImGui::EndMenuBar();
            }
        }
        ImGui::End();
        ImGui::PopStyleVar(1);
        ImGui::PopStyleColor(2);
        ImGui::PopFont();

        if (openSettings) {
            ImGui::OpenPopup("Settings");
        }
        ImGui::SetNextWindowSizeConstraints(ImVec2(200, 200), ImVec2(FLT_MAX, FLT_MAX));
        if (ImGui::BeginPopupModal("Settings")) {
            settings::settingsDialog(_render->getNeoneuronApplication());
            ImGui::EndPopup();
        }

        demo();
    }
} // namespace neoneuron
