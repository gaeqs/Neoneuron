//
// Created by gaeqs on 11/10/24.
//

#include "NeoneuronTopBar.h"

#ifdef WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#endif

#include <imgui_internal.h>
#include <nfd.hpp>
#include <nfd_glfw3.h>
#include <neoneuron/loader/SWCLoader.h>
#include <neoneuron/loader/XMLLoader.h>

#include <neoneuron/render/NeoneuronRender.h>
#include <neoneuron/structure/NeuronTransform.h>
#include <neoneuron/structure/prototype/NeuronProperties.h>

#include "settings/NeoneuronUiSettings.h"
#include "style/Fonts.h"

namespace neoneuron {
    void NeoneuronTopBar::openSWC() const {
        auto* app = dynamic_cast<neon::vulkan::VKApplication*>(getApplication()->getImplementation());

        nfdwindowhandle_t handle;
        NFD_GetNativeWindowFromGLFWWindow(app->getWindow(), &handle);


        NFD::UniquePath outPath = NULL;
        nfdresult_t result = NFD::OpenDialog(
            outPath,
            nullptr,
            0,
            nullptr,
            handle
        );
        std::string file;
        if (result == NFD_OKAY) {
            file = std::string(outPath.get());
        } else if (result == NFD_CANCEL) {
            return;
        } else {
            getLogger().error(neon::MessageBuilder()
                .print("Error while choosing file: ")
                .print(NFD_GetError()));
            return;
        }

        std::filesystem::path path(file);

        std::ifstream ss(path);
        SWCLoader loader(ss);
        loader.setFileName(path.filename().string());
        auto loaderResult = loader.build(_render->getNeuronScene()->getNeuronsAmount());

        if (!loaderResult.isOk()) {
            getLogger().error(loaderResult.getError());
            return;
        }

        for (auto& prototype: std::move(loaderResult.getResult())) {
            _render->getNeuronScene()->addNeuron(std::move(prototype));
        }
        _render->focusScene();
    }

    void NeoneuronTopBar::openXML() const {
        auto* app = dynamic_cast<neon::vulkan::VKApplication*>(getApplication()->getImplementation());

        nfdwindowhandle_t handle;
        NFD_GetNativeWindowFromGLFWWindow(app->getWindow(), &handle);

        NFD::UniquePath outPath = NULL;
        nfdresult_t result = NFD::OpenDialog(
            outPath,
            nullptr,
            0,
            nullptr,
            handle
        );
        std::string file;
        if (result == NFD_OKAY) {
            file = std::string(outPath.get());
        } else if (result == NFD_CANCEL) {
            return;
        } else {
            getLogger().error(neon::MessageBuilder()
                .print("Error while choosing file: ")
                .print(NFD_GetError()));
            return;
        }

        std::filesystem::path path(file);
        neon::DirectoryFileSystem fileSystem(path.parent_path());

        std::ifstream ss(path);
        XMLLoader loader(&fileSystem, ss);
        auto loaderResult = loader.build(0);

        if (!loaderResult.isOk()) {
            getLogger().error(loaderResult.getError());
            return;
        }

        std::cout << loaderResult.getResult().size() << std::endl;
        std::cout << loaderResult.getResult().size() << std::endl;
        std::cout << loaderResult.getResult().size() << std::endl;
        std::cout << loaderResult.getResult().size() << std::endl;
        std::cout << loaderResult.getResult().size() << std::endl;
        for (auto& prototype: loaderResult.getResult()) {
            _render->getNeuronScene()->addNeuron(std::move(prototype));
        }
        _render->focusScene();
    }

    void NeoneuronTopBar::toolsMenu() const {
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

    void NeoneuronTopBar::demo() const {
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

    NeoneuronTopBar::NeoneuronTopBar(NeoneuronRender* render)
        : _render(render) {}

    void NeoneuronTopBar::onStart() {}

    void NeoneuronTopBar::onPreDraw() {
        bool openSettings = false;

        fonts::imGuiPushFont(fonts::SS3_20);
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                fonts::imGuiPushFont(fonts::SS3_18);
                if (ImGui::MenuItem("Open SWC File", "Ctrl+O")) {
                    openSWC();
                }
                if (ImGui::MenuItem("Open XML File", "Ctrl+O")) {
                    openXML();
                }
                if (ImGui::MenuItem("Settings", "Ctrl+S")) {
                    openSettings = true;
                }
                ImGui::PopFont();
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Tools")) {
                fonts::imGuiPushFont(fonts::SS3_18);
                toolsMenu();
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

        ImGuiViewportP* viewport = (ImGuiViewportP*)(void*)ImGui::GetMainViewport();
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings |
                                        ImGuiWindowFlags_MenuBar;
        float height = ImGui::GetFrameHeight();

        if (ImGui::BeginViewportSideBar("##Status", viewport, ImGuiDir_Down, height, window_flags)) {
            if (ImGui::BeginMenuBar()) {
                ImGui::Text("Happy status bar");
                ImGui::Text("Happy status bar");
                ImGui::EndMenuBar();
            }
            ImGui::End();
        }
    }
}
