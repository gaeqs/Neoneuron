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

#include "ActionSave.h"

#include <neoneuron/application/NeoneuronApplication.h>
#include <neoneuron/render/NeoneuronRender.h>
#include <neoneuron/render/complex/ComplexNeuronRepresentation.h>
#undef max

namespace neoneuron
{
    void ActionSave::run()
    {
        auto selection = _application->getSelector().getSelectedNeurons();
        getTaskRunner().launchCoroutine(saveCoroutine(std::move(selection)));
    }

    neon::Coroutine<> ActionSave::saveCoroutine(std::unordered_set<GID> gids)
    {
        constexpr size_t VALUES_PER_SEGMENT = 64;
        constexpr size_t VALUES_PER_JOINT = 68;
        constexpr size_t VALUES_PER_SOMA = 128 * 64 + 8 * 32;

        ComplexNeuronRepresentation* representation = nullptr;

        for (auto rep : _application->getRender().getRepresentations()) {
            if (auto* complex = dynamic_cast<ComplexNeuronRepresentation*>(rep)) {
                representation = complex;
                break;
            }
        }

        if (representation == nullptr) {
            co_return;
        }

        auto& repo = _application->getRepository();

        for (GID gid : gids) {
            auto optional = repo.getNeuronAndDataset(gid);
            if (!optional.has_value()) {
                continue;
            }
            auto [dataset, neuron] = *optional;
            auto typeProp = dataset->getDataset().getProperties().getPropertyUID(mindset::PROPERTY_NEURITE_TYPE);

            if (!neuron->getMorphology().has_value()) {
                continue;
            }

            auto morph = neuron->getMorphology().value();

            mindset::UID maxSegment = 0;
            for (auto segment : morph->getNeuritesUIDs()) {
                maxSegment = std::max(maxSegment, segment);
            }

            auto& ubo = representation->getUBO();
            ubo->clearData(7);
            ubo->clearData(8);
            representation->getRender()->getRenderData().savingNeuron = gid.internalId;
            co_yield neon::WaitForNextFrame();

            representation->getRender()->getRenderData().savingNeuron = std::numeric_limits<uint32_t>::max();

            ubo->transferDataFromGPU(7);
            ubo->transferDataFromGPU(8);

            const auto* data = static_cast<const rush::Vec4f*>(ubo->fetchData(7));
            const auto* indices = static_cast<const rush::Vec4i*>(ubo->fetchData(8));

            std::vector<rush::Vec3f> vertices;
            for (auto segment : morph->getNeurites()) {
                bool hasType = typeProp.has_value();
                bool isSoma = hasType && segment->getProperty<mindset::NeuriteType>(typeProp.value()) ==
                                             mindset::NeuriteType::SOMA;

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

    ActionSave::ActionSave(NeoneuronApplication* application) :
        ModalComponent("Save neuron model", true),
        _application(application)
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
