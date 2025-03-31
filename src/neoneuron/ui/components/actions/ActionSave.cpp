//
// Created by gaeqs on 14/02/2025.
//

#include "ActionSave.h"

#include <neoneuron/application/NeoneuronApplication.h>
#include <neoneuron/render/NeoneuronRender.h>
#include <neoneuron/render/complex/ComplexNeuronRepresentation.h>
#undef max

namespace neoneuron
{
    void ActionSave::run()
    {
        auto selection = _scene->getSelector().getSelectedNeurons();
        getTaskRunner().launchCoroutine(saveCoroutine(std::move(selection)));
    }

    neon::Coroutine<> ActionSave::saveCoroutine(std::unordered_set<mindset::UID> uids)
    {
        constexpr size_t VALUES_PER_SEGMENT = 64;
        constexpr size_t VALUES_PER_JOINT = 68;
        constexpr size_t VALUES_PER_SOMA = 128 * 64 + 8 * 32;

        auto* scene = dynamic_cast<ComplexNeuronRepresentation*>(_scene);
        if (scene == nullptr) {
            co_return;
        }

        auto& dataset = _scene->getRender()->getNeoneuronApplication()->getDataset();
        auto typeProp = dataset.getProperties().getPropertyUID(mindset::PROPERTY_NEURITE_TYPE);

        for (mindset::UID uid : uids) {
            auto optional = dataset.getNeuron(uid);
            if (!optional.has_value()) {
                continue;
            }
            auto* neuron = optional.value();

            if (!neuron->getMorphology().has_value()) {
                continue;
            }

            auto morph = neuron->getMorphology().value();

            mindset::UID maxSegment = 0;
            for (auto segment : morph->getNeuritesUIDs()) {
                maxSegment = std::max(maxSegment, segment);
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

            std::vector<rush::Vec3f> vertices;
            for (auto segment : morph->getNeurites()) {
                bool hasType = typeProp.has_value();
                bool isSoma =
                    hasType && segment->getProperty<mindset::NeuriteType>(typeProp.value()) == mindset::NeuriteType::SOMA;

                if (!isSoma) {
                    // Segment
                    size_t offset = segment->getUID() * VALUES_PER_SEGMENT;
                    for (size_t i = 0; i < VALUES_PER_SEGMENT; ++i) {
                        rush::Vec4i triangle = indices[i + offset];
                        if (triangle.w() == 0) {
                            continue;
                        }

                        auto x = rush::Vec3f(data[triangle.x()]);
                        auto y = rush::Vec3f(data[triangle.y()]);
                        auto z = rush::Vec3f(data[triangle.z()]);

                        vertices.push_back(x);
                        vertices.push_back(y);
                        vertices.push_back(z);
                    }

                    // Joint
                    offset = VALUES_PER_SEGMENT * (maxSegment + 1) + VALUES_PER_JOINT * segment->getUID();
                    for (size_t i = 0; i < VALUES_PER_JOINT; ++i) {
                        rush::Vec4i triangle = indices[i + offset];
                        if (triangle.w() == 0) {
                            continue;
                        }

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
                        if (triangle.w() == 0) {
                            continue;
                        }

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
            for (auto& value : vertices) {
                out << "v " << value.x() << " " << value.y() << " " << value.z() << std::endl;
            }

            for (size_t i = 1; i <= vertices.size(); i += 3) {
                out << "f " << i << " " << i + 1 << " " << i + 2 << std::endl;
            }

            std::cout << "CREATED" << std::endl;
            out.close();
        }
    }

    ActionSave::ActionSave(AbstractNeuronRepresentation* scene) :
        ModalComponent("Save neuron model", true),
        _scene(scene)
    {
    }

    void ActionSave::onModalDraw()
    {
    }

    void ActionSave::actionButton(ImVec2 recommendedSize)
    {
        if (ImGui::Button("Run", recommendedSize)) {
            run();
            ImGui::CloseCurrentPopup();
        }
    }
} // namespace neoneuron
