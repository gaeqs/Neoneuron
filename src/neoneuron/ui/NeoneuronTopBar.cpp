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

#include <neoneuron/render/NeoneuronRender.h>

#include "settings/NeoneuronUiSettings.h"
#include "style/Fonts.h"

namespace neoneuron {
    void NeoneuronTopBar::openFile() const {
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

        std::ifstream ss(file);
        SWCLoader loader(ss);
        auto loaderResult = loader.build(_render->getNeuronScene()->getNeuronsAmount());

        if (!loaderResult.isOk()) {
            getLogger().error(loaderResult.getError());
            return;
        }

        PrototypeNeuron prototype = std::move(loaderResult.getResult());
        _render->getNeuronScene()->addNeuron(prototype);
        _render->focusScene();
    }

    NeoneuronTopBar::NeoneuronTopBar(NeoneuronRender* render)
        : _render(render) {}

    void NeoneuronTopBar::onStart() {}

    void NeoneuronTopBar::onPreDraw() {
        bool openSettings = false;

        ImGui::ShowDemoWindow();
        fonts::imGuiPushFont(fonts::SS3_20);
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                fonts::imGuiPushFont(fonts::SS3_18);
                if (ImGui::MenuItem("Open SWC File", "Ctrl+O")) {
                    openFile();
                }
                if (ImGui::MenuItem("Settings", "Ctrl+S")) {
                    openSettings = true;
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
            settings::settingsDialog();
            ImGui::EndPopup();
        }

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
