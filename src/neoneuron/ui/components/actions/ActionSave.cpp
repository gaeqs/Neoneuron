//
// Created by gaeqs on 14/02/2025.
//

#include "ActionSave.h"

#include <neoneuron/render/NeoneuronRender.h>
#include <neoneuron/render/complex/ComplexNeuronScene.h>
#undef max

namespace neoneuron {
    void ActionSave::run() {
        auto selection = _scene->getSelector().getSelectedNeurons();
        getTaskRunner().launchCoroutine(saveCoroutine(std::move(selection)));
    }

    neon::Coroutine<> ActionSave::saveCoroutine(std::unordered_set<UID> uids) {
        constexpr size_t VALUES_PER_SEGMENT = 64;

        auto* scene = dynamic_cast<ComplexNeuronScene*>(_scene);
        if (scene == nullptr) co_return;

        for (UID uid: uids) {
            auto optional = _scene->findPrototypeNeuron(uid);
            if (!optional.has_value()) continue;
            auto* neuron = optional.value();

            auto& ubo = scene->getUBO();
            ubo->clearData(7);
            ubo->clearData(8);
            scene->getRender()->getRenderData().savingNeuron = uid;
            co_yield neon::WaitForNextFrame();


            scene->getRender()->getRenderData().savingNeuron = std::numeric_limits<uint32_t>::max();

            ubo->transferDataFromGPU(7);
            ubo->transferDataFromGPU(8);

            const rush::Vec4f* data = static_cast<const rush::Vec4f*>(ubo->fetchData(7));
            const rush::Vec4i* indices = static_cast<const rush::Vec4i*>(ubo->fetchData(8));

            std::vector<rush::Vec3f> triangles;
            for (auto& segment: neuron->getSegments()) {
                size_t offset = segment.getId() * VALUES_PER_SEGMENT;
                for (size_t i = 0; i < VALUES_PER_SEGMENT; ++i) {
                    rush::Vec4i triangle = indices[i + offset];
                    if (triangle.w() == 0) continue;
                    triangles.push_back(rush::Vec3f(data[triangle.x()]));
                    triangles.push_back(rush::Vec3f(data[triangle.y()]));
                    triangles.push_back(rush::Vec3f(data[triangle.z()]));
                }
            }

            std::cout << triangles.size() << std::endl;
        }
    }

    ActionSave::ActionSave(AbstractNeuronScene* scene)
        : ModalComponent("Save neuron model", true), _scene(scene) {}

    void ActionSave::onModalDraw() {}

    void ActionSave::actionButton(ImVec2 recommendedSize) {
        if (ImGui::Button("Run", recommendedSize)) {
            run();
            ImGui::CloseCurrentPopup();
        }
    }
}
