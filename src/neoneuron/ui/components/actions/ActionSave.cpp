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
        constexpr size_t VALUES_PER_JOINT = 68;
        constexpr size_t VALUES_PER_SOMA = 128 * 64 + 8 * 32;


        auto* scene = dynamic_cast<ComplexNeuronScene*>(_scene);
        if (scene == nullptr) co_return;

        for (UID uid: uids) {
            auto optional = _scene->findPrototypeNeuron(uid);
            if (!optional.has_value()) continue;
            auto* neuron = optional.value();

            UID maxSegment = 0;
            for (auto& segment: neuron->getSegments()) {
                maxSegment = std::max(maxSegment, segment.getId());
            }

            auto& ubo = scene->getUBO();
            ubo->clearData(7);
            ubo->clearData(8);
            scene->getRender()->getRenderData().savingNeuron = uid;
            co_yield neon::WaitForNextFrame();

            scene->getRender()->getRenderData().savingNeuron = std::numeric_limits<uint32_t>::max();

            ubo->transferDataFromGPU(7);
            ubo->transferDataFromGPU(8);

            const auto* data = static_cast<const rush::Vec4f*>(ubo->fetchData(7));
            const auto* indices = static_cast<const rush::Vec4i*>(ubo->fetchData(8));

            auto segmentTypeUID = neuron->getPropertyUID(PROPERTY_TYPE);

            std::vector<rush::Vec3f> vertices;
            for (auto& segment: neuron->getSegments()) {
                bool hasType = segmentTypeUID.has_value();
                bool isSoma = hasType && segment.getProperty<SegmentType>(segmentTypeUID.value()) == SegmentType::SOMA;

                if (!isSoma) {
                    // Segment
                    size_t offset = segment.getId() * VALUES_PER_SEGMENT;
                    for (size_t i = 0; i < VALUES_PER_SEGMENT; ++i) {
                        rush::Vec4i triangle = indices[i + offset];
                        if (triangle.w() == 0) continue;

                        auto x = rush::Vec3f(data[triangle.x()]);
                        auto y = rush::Vec3f(data[triangle.y()]);
                        auto z = rush::Vec3f(data[triangle.z()]);

                        vertices.push_back(x);
                        vertices.push_back(y);
                        vertices.push_back(z);
                    }

                    // Joint
                    offset = VALUES_PER_SEGMENT * (maxSegment + 1) + VALUES_PER_JOINT * segment.getId();
                    for (size_t i = 0; i < VALUES_PER_JOINT; ++i) {
                        rush::Vec4i triangle = indices[i + offset];
                        if (triangle.w() == 0) continue;

                        auto x = rush::Vec3f(data[triangle.x()]);
                        auto y = rush::Vec3f(data[triangle.y()]);
                        auto z = rush::Vec3f(data[triangle.z()]);

                        vertices.push_back(x);
                        vertices.push_back(y);
                        vertices.push_back(z);
                    }
                } else {
                    // Soma
                    size_t offset = VALUES_PER_SEGMENT * (maxSegment + 1) + VALUES_PER_JOINT * (maxSegment + 1);
                    for (size_t i = 0; i < VALUES_PER_SOMA; ++i) {
                        rush::Vec4i triangle = indices[i + offset];
                        if (triangle.w() == 0) continue;

                        auto x = rush::Vec3f(data[triangle.x()]);
                        auto y = rush::Vec3f(data[triangle.y()]);
                        auto z = rush::Vec3f(data[triangle.z()]);

                        vertices.push_back(x);
                        vertices.push_back(y);
                        vertices.push_back(z);
                    }
                }
            }

            std::cout << vertices.size() << std::endl;

            std::ofstream out("test.obj");
            for (auto& value: vertices) {
                out << "v " << value.x() << " " << value.y() << " " << value.z() << std::endl;
            }

            for (size_t i = 1; i <= vertices.size(); i += 3) {
                out << "f " << i << " " << i + 1 << " " << i + 2 << std::endl;
            }

            std::cout << "CREATED" << std::endl;
            out.close();
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
