//
// Created by gaeqs on 11/10/24.
//

#include "NeoneuronTopBar.h"

#ifdef WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#endif

#include <nfd.hpp>
#include <nfd_glfw3.h>
#include <neoneuron/loader/SWCLoader.h>

#include <neoneuron/render/NeoneuronRender.h>
#include <neoneuron/render/NeuronScene.h>

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
        auto loaderResult = loader.build(1);

        if (!loaderResult.isOk()) {
            getLogger().error(loaderResult.getError());
            return;
        }

        Neuron neuron = std::move(loaderResult.getResult());
        _scene->addNeuron(std::move(neuron));
    }

    NeoneuronTopBar::NeoneuronTopBar(NeoneuronRender& render)
        : _scene(&render.getNeuronScene()) {}

    void NeoneuronTopBar::onStart() {}

    void NeoneuronTopBar::onPreDraw() {
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("Open SWC File", "Ctrl+O")) {
                    openFile();
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
    }
}
