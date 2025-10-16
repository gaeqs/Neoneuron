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

#include "NeuronParticleRepresentationNode.h"

#include <neoneuron/render/particle/ParticleNeuronRepresentation.h>
#include <neoneuron/structure/RepositoryView.h>
#include <neoneuron/ui/components/nodes/EventSequenceNode.h>

namespace neoneuron
{

    void NeuronParticleRepresentationNode::drawProgressBar(ParticleNeuronRepresentation* ptr)
    {
        auto used = static_cast<float>(ptr->getUsedInstanceMemory()) / 1024.0f / 1024.0f;
        auto allocated = static_cast<float>(ptr->getAllocatedInstanceMemory()) / 1024.0f / 1024.0f;
        std::string text = std::format("{:.2f}/{:.2f} MiB", used, allocated);
        ImGui::ProgressBar(ptr->getUsedInstanceMemoryPercentage(), ImVec2(200, 0), text.c_str());
        ImGui::Text("Bytes: %d", ptr->getUsedInstanceMemory());
    }

    NeuronParticleRepresentationNode::NeuronParticleRepresentationNode(NeoneuronApplication* application) :
        Node("Neurons as particles representation"),
        _application(application)
    {
        defineInput<RepositoryView>("Data", true);
        defineInput<Viewport*>("Viewport", true);
        defineInput<std::shared_ptr<NeuronColorAndScaleSE>>("Color and scale", true);

        _representation = _application->getRender().addRepresentation<ParticleNeuronRepresentation>();
        defineOutput<std::weak_ptr<AbstractNeuronRepresentation>>("Representation", _representation);
    }

    NeuronParticleRepresentationNode::~NeuronParticleRepresentationNode()
    {
        sendOutput("Representation", std::any());
        if (auto ptr = _representation.lock()) {
            _application->getRender().removeRepresentation(ptr.get());
        }
    }

    void NeuronParticleRepresentationNode::renderBody()
    {
        auto ptr = _representation.lock();
        if (!ptr) {
            return;
        }

        drawProgressBar(ptr.get());
    }

    void NeuronParticleRepresentationNode::onInputChange(const std::string& name, const std::any& value)
    {
        auto ptr = _representation.lock();
        if (!ptr) {
            return;
        }

        if (name == "Data") {
            auto data = getMultipleInputs<RepositoryView>("Data");
            if (!data.has_value()) {
                ptr->clearData();
                return;
            }

            auto combined = RepositoryView::combine(data.value());
            if (combined.has_value()) {
                ptr->refreshData(combined.value());
            } else {
                ptr->clearData();
            }
        } else if (name == "Viewport") {
            auto viewports = getMultipleInputs<Viewport*>("Viewport");
            if (!viewports.has_value()) {
                ptr->setViewports({});
                return;
            }

            auto& vec = viewports.value();

            std::unordered_set<const Viewport*> set(vec.begin(), vec.end());
            ptr->setViewports(set);
        } else if (name == "Color and scale") {
            if (auto se = getInput<std::shared_ptr<NeuronColorAndScaleSE>>("Color and scale")) {
                ptr->setColorAndScale(*se);
            } else {
                ptr->setColorAndScale(nullptr);
            }
        }
    }

    NodeFactory NeuronParticleRepresentationNode::createFactory()
    {
        return NodeFactory("Neurons as particles representation", [](ImBlueprint::Editor& editor, auto* app) {
            return editor.addNode<NeuronParticleRepresentationNode>(app);
        });
    }
} // namespace neoneuron
