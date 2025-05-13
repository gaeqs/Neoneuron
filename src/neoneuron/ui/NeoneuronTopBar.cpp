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

#ifdef WIN32
    #define GLFW_EXPOSE_NATIVE_WIN32
#endif

#include <imgui_internal.h>
#include <neoneuron/loader/SceneLoader.h>

#include <neoneuron/render/NeoneuronRender.h>
#include <neoneuron/render/complex/ComplexNeuronRepresentation.h>

#include "components/NeoneuronUIOpenFile.h"
#include "components/actions/ActionSave.h"
#include "components/actions/ActionShuffle.h"
#include "components/NeoneuronUIAbout.h"
#include "neon/util/dialog/Dialogs.h"
#include "settings/NeoneuronUiSettings.h"
#include "style/Fonts.h"

namespace neoneuron
{
    void NeoneuronTopBar::openFile() const
    {
        neon::OpenFileDialogInfo info;
        info.application = getApplication();
        auto result = neon::openFileDialog(info);

        std::filesystem::path path = result[0];
        auto fileSystem = std::make_unique<neon::DirectoryFileSystem>(path.parent_path());
        auto optional = fileSystem->readFile(path.filename());
        if (!optional.has_value()) {
            return;
        }
        _render->getRoom()->newGameObject()->newComponent<NeoneuronUiOpenFile>(
            _render->getNeoneuronApplication(), std::move(fileSystem), path, std::move(optional.value()));
    }

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
        static const auto GLOBAL_PARAMS = NeoneuronFiles::SETTINGS_TOOL_GLOBAL_PARAMETERS;
        static const auto DEBUG = NeoneuronFiles::SETTINGS_TOOL_DEBUG;
        static const auto DEMO = NeoneuronFiles::SETTINGS_TOOL_DEMO;
        auto& files = _render->getNeoneuronApplication()->getFiles();
        auto& s = files.getSettings();

        bool globalParameters = s.value(GLOBAL_PARAMS, false);
        bool debug = s.value(DEBUG, false);
        bool demo = s.value(DEMO, false);

        if (ImGui::MenuItem("Global parameters", nullptr, &globalParameters)) {
            s[GLOBAL_PARAMS] = globalParameters;
            files.signalSettingsChange(GLOBAL_PARAMS);
        }

        if (ImGui::MenuItem("Debug", nullptr, &debug)) {
            s[DEBUG] = debug;
            files.signalSettingsChange(DEBUG);
        }

        if (ImGui::MenuItem("ImGUI Demo", nullptr, &demo)) {
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
        if (ImGui::MenuItem("Focus scene")) {
            _render->focusScene();
        }
        if (ImGui::MenuItem("Shuffle")) {
            _render->getRoom()->newGameObject()->newComponent<ActionShuffle>(app);
        }
        if (ImGui::MenuItem("Save neuron model")) {
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

    NeoneuronTopBar::NeoneuronTopBar(NeoneuronRender* render) :
        _render(render)
    {
    }

    void NeoneuronTopBar::onStart()
    {
    }

    void NeoneuronTopBar::onPreDraw()
    {
        bool openSettings = false;

        fonts::imGuiPushFont(fonts::SS3_20);
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                fonts::imGuiPushFont(fonts::SS3_18);
                if (ImGui::MenuItem("Open file", "Ctrl+O")) {
                    openFile();
                }
                if (ImGui::MenuItem("Close scene")) {
                    _render->getNeoneuronApplication()->getRepository().clear();
                }
                if (ImGui::MenuItem("Save scene")) {
                    saveFile(saveScene(_render).dump(4));
                }
                if (ImGui::MenuItem("Settings", "Ctrl+S")) {
                    openSettings = true;
                }
                ImGui::PopFont();
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("View")) {
                fonts::imGuiPushFont(fonts::SS3_18);
                viewMenu();
                ImGui::PopFont();
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Tools")) {
                fonts::imGuiPushFont(fonts::SS3_18);
                toolsMenu();
                ImGui::PopFont();
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Actions")) {
                fonts::imGuiPushFont(fonts::SS3_18);
                actionsMenu();
                ImGui::PopFont();
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Help")) {
                fonts::imGuiPushFont(fonts::SS3_18);
                if (ImGui::MenuItem("About")) {
                    _render->getRoom()->newGameObject()->newComponent<NeoneuronUIAbout>(
                        _render->getNeoneuronApplication());
                }
                ImGui::PopFont();
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
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
