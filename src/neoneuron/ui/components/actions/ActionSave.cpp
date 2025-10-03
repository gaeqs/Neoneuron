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
        if (selection.empty()) {
            for (const auto& gid : _application->getRepository().getNeurons() | std::views::keys) {
                selection.insert(gid);
            }
        }
        getTaskRunner().launchCoroutine(saveCoroutine(std::move(selection)));
    }

    neon::Coroutine<> ActionSave::saveCoroutine(std::unordered_set<GID> gids)
    {
        constexpr size_t VALUES_PER_SEGMENT = 64;
        constexpr size_t VALUES_PER_JOINT = 68;
        constexpr size_t VALUES_PER_SOMA = 128 * 64 + 8 * 32;

        std::filesystem::path folder = "exported";
        if (!std::filesystem::exists(folder)) {
            if (!std::filesystem::create_directory(folder)) {
                neon::error() << "Couldn't create the folder 'exported'";
                co_return;
            }
        } else if (!std::filesystem::is_directory(folder)) {
            neon::error() << "Couldn't create the folder 'exported'";
            co_return;
        }

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

        std::ofstream stats(folder / "stats.csv", std::ios::out | std::ios::app);

        for (GID gid : gids) {
            auto optional = repo.getNeuronAndDataset(gid);
            if (!optional.has_value()) {
                continue;
            }
            auto [dataset, neuron] = *optional;

            if (!neuron->getMorphology().has_value()) {
                continue;
            }

            auto morph = neuron->getMorphology().value();

            std::string outputName;
            if (auto pathProp = dataset->getDataset().getProperties().getPropertyUID(mindset::PROPERTY_PATH)) {
                if (auto pathString = morph->getProperty<std::string>(*pathProp)) {
                    std::filesystem::path path = *pathString;
                    outputName = path.filename();
                }
            }
            if (outputName.empty()) {
                outputName = std::format("{}_{}", gid.datasetId, gid.internalId);
            }

            neon::debug() << "Saving " << outputName;

            auto outputPath = folder / (outputName + ".obj");
            if (std::filesystem::exists(outputPath)) {
                neon::debug() << "- Skip";
                continue;
            }

            mindset::UID maxSegment = 0;
            for (auto segment : morph->getNeuritesUIDs()) {
                maxSegment = std::max(maxSegment, segment);
            }

            auto& ubo = representation->getUBO();
            ubo->clearData(ComplexNeuronRepresentation::SAVING_VERTICES_DATA_BINDING);
            ubo->clearData(ComplexNeuronRepresentation::SAVING_INDICES_DATA_BINDING);
            representation->getRender()->getRenderData().savingNeuron = gid.internalId;
            co_yield neon::WaitForNextFrame();

            representation->getRender()->getRenderData().savingNeuron = std::numeric_limits<uint32_t>::max();

            ubo->transferDataFromGPU(ComplexNeuronRepresentation::SAVING_VERTICES_DATA_BINDING);
            ubo->transferDataFromGPU(ComplexNeuronRepresentation::SAVING_INDICES_DATA_BINDING);

            const auto* data = static_cast<const rush::Vec4f*>(
                ubo->fetchData(ComplexNeuronRepresentation::SAVING_VERTICES_DATA_BINDING));
            const auto* indices = static_cast<const rush::Vec4i*>(
                ubo->fetchData(ComplexNeuronRepresentation::SAVING_INDICES_DATA_BINDING));

            rush::Mesh<3, float, void> mesh;

            for (auto segment : morph->getNeurites()) {
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

                    mesh.vertices.push_back(rush::Vertex<3, float, void>(x));
                    mesh.vertices.push_back(rush::Vertex<3, float, void>(y));
                    mesh.vertices.push_back(rush::Vertex<3, float, void>(z));
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

                    mesh.vertices.push_back(rush::Vertex<3, float, void>(x));
                    mesh.vertices.push_back(rush::Vertex<3, float, void>(y));
                    mesh.vertices.push_back(rush::Vertex<3, float, void>(z));
                }
            }

            if (morph->getSoma()) {
                size_t offset = VALUES_PER_SEGMENT * (maxSegment + 1) + VALUES_PER_JOINT * (maxSegment + 1);
                for (size_t i = 0; i < VALUES_PER_SOMA; ++i) {
                    rush::Vec4i triangle = indices[i + offset];
                    if (triangle.w() == 0) {
                        continue;
                    }

                    auto x = rush::Vec3f(data[triangle.x()]);
                    auto y = rush::Vec3f(data[triangle.y()]);
                    auto z = rush::Vec3f(data[triangle.z()]);

                    mesh.vertices.push_back(rush::Vertex<3, float, void>(x));
                    mesh.vertices.push_back(rush::Vertex<3, float, void>(y));
                    mesh.vertices.push_back(rush::Vertex<3, float, void>(z));
                }
            }

            for (size_t i = 0; i < mesh.vertices.size(); i += 3) {
                mesh.indices.push_back(i);
                mesh.indices.push_back(i + 1);
                mesh.indices.push_back(i + 2);
            }

            neon::debug() << "- Welding...";
            neon::debug() << "- Welded vertices: " << mesh.weld();
            neon::debug() << "- Removing unused vertices...";
            neon::debug() << "- Removed unused vertices: " << mesh.removeUnusedVertices();
            neon::debug() << "- Writing stats";

            stats << outputName << ","
                  << mesh.vertices.size() * sizeof(rush::Vec3f) + mesh.indices.size() * sizeof(unsigned int)
                  << std::endl;

            std::ofstream out(outputPath);
            for (auto& value : mesh.vertices) {
                out << "v " << value.position.x() << " " << value.position.y() << " " << value.position.z()
                    << std::endl;
            }

            for (size_t i = 0; i < mesh.indices.size(); i += 3) {
                out << "f " << mesh.indices[i] + 1 << " " << mesh.indices[i + 1] + 1 << " " << mesh.indices[i + 2] + 1
                    << std::endl;
            }

            out.close();
        }

        stats.close();
    }

    ActionSave::ActionSave(NeoneuronApplication* application) :
        ModalComponent("Save neuron model", true),
        _application(application)
    {
    }

    void ActionSave::onModalDraw()
    {
    }

    void ActionSave::actionButton(ImGui::neon::LinearLayout& layout)
    {
        if (layout.button("Run", ImVec2(120.0f, 0.0f))) {
            run();
            ImGui::CloseCurrentPopup();
        }
    }
} // namespace neoneuron
