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
#include <nfd.hpp>
#include <nfd_glfw3.h>
#include <neoneuron/loader/SceneLoader.h>

#include <neoneuron/render/NeoneuronRender.h>
#include <neoneuron/render/complex/ComplexNeuronRepresentation.h>

#include "components/NeoneuronUIOpenFile.h"
#include "components/actions/ActionSave.h"
#include "components/actions/ActionShuffle.h"
#include "settings/NeoneuronUiSettings.h"
#include "style/Fonts.h"

namespace neoneuron
{
    void NeoneuronTopBar::openFile() const
    {
        auto* app = dynamic_cast<neon::vulkan::VKApplication*>(getApplication()->getImplementation());

        nfdwindowhandle_t handle;
        NFD_GetNativeWindowFromGLFWWindow(app->getWindow(), &handle);

        NFD::UniquePath outPath = NULL;
        nfdresult_t result = NFD::OpenDialog(outPath, nullptr, 0, nullptr, handle);
        std::string file;
        if (result == NFD_OKAY) {
            file = std::string(outPath.get());
        } else if (result == NFD_CANCEL) {
            return;
        } else {
            getLogger().error(neon::MessageBuilder().print("Error while choosing file: ").print(NFD_GetError()));
            return;
        }

        std::filesystem::path path(file);
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
        auto* app = dynamic_cast<neon::vulkan::VKApplication*>(getApplication()->getImplementation());

        nfdwindowhandle_t handle;
        NFD_GetNativeWindowFromGLFWWindow(app->getWindow(), &handle);

        nfdu8filteritem_t filters = {"JSON", "json"};
        NFD::UniquePath outPath = NULL;
        nfdresult_t result = NFD::SaveDialog(outPath, &filters, 0, nullptr, "scene.json", handle);
        std::string file;
        if (result == NFD_OKAY) {
            file = std::string(outPath.get());
        } else if (result == NFD_CANCEL) {
            return;
        } else {
            getLogger().error(neon::MessageBuilder().print("Error while choosing file: ").print(NFD_GetError()));
            return;
        }

        std::filesystem::path path(file);

        std::ofstream out(path);
        if (!out) {
            return;
        }
        out << data;
        out.close();
    }

    void NeoneuronTopBar::toolsMenu() const
    {
        static const auto GLOBAL_PARAMS = NeoneuronApplication::SETTINGS_TOOL_GLOBAL_PARAMETERS;
        static const auto DEBUG = NeoneuronApplication::SETTINGS_TOOL_DEBUG;
        static const auto DEMO = NeoneuronApplication::SETTINGS_TOOL_DEMO;
        auto& s = _render->getNeoneuronApplication()->getSettings();

        bool globalParameters = s.value(GLOBAL_PARAMS, false);
        bool debug = s.value(DEBUG, false);
        bool demo = s.value(DEMO, false);

        if (ImGui::MenuItem("Global parameters", nullptr, &globalParameters)) {
            s[GLOBAL_PARAMS] = globalParameters;
            _render->getNeoneuronApplication()->signalSettingsChange(GLOBAL_PARAMS);
        }

        if (ImGui::MenuItem("Debug", nullptr, &debug)) {
            s[DEBUG] = debug;
            _render->getNeoneuronApplication()->signalSettingsChange(DEBUG);
        }

        if (ImGui::MenuItem("ImGUI Demo", nullptr, &demo)) {
            s[DEMO] = demo;
            _render->getNeoneuronApplication()->signalSettingsChange(DEMO);
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
        auto& s = _render->getNeoneuronApplication()->getSettings();
        bool opened = s.value(NeoneuronApplication::SETTINGS_TOOL_DEMO, false);
        bool keepOpen = true;
        if (opened) {
            ImGui::ShowDemoWindow(&keepOpen);
        }
        if (opened && !keepOpen) {
            s[NeoneuronApplication::SETTINGS_TOOL_DEMO] = false;
            _render->getNeoneuronApplication()->signalSettingsChange(NeoneuronApplication::SETTINGS_TOOL_DEMO);
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
