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

#include "MorphologyRepresentationNode.h"

#include <neoneuron/ui/style/MaterialSymbols.h>
#include <neoneuron/ui/style/NodeStyles.h>
#include <neoneuron/render/complex/ComplexNeuronRepresentation.h>
#include <neoneuron/structure/RepositoryView.h>

namespace neoneuron
{

    void MorphologyRepresentationNode::drawProgressBar(ComplexNeuronRepresentation* ptr)
    {
        auto used = static_cast<float>(ptr->getUsedInstanceMemory()) / 1024.0f / 1024.0f;
        auto allocated = static_cast<float>(ptr->getAllocatedInstanceMemory()) / 1024.0f / 1024.0f;
        std::string text = std::format("{:.2f}/{:.2f} MiB", used, allocated);
        ImGui::ProgressBar(ptr->getUsedInstanceMemoryPercentage(), ImVec2(200, 0), text.c_str());
        ImGui::Text("Bytes: %d", ptr->getUsedInstanceMemory());
    }

    MorphologyRepresentationNode::MorphologyRepresentationNode(NeoneuronApplication* application) :
        NeoneuronNode("Neuron morphology representation", ICON_MS_FORMAT_PAINT, NODE_STYLE_REPRESENTATION),
        _application(application)
    {
        defineInput<RepositoryView>("Data", true);
        defineInput<Viewport*>("Viewport", true);
        defineInput<std::shared_ptr<NeuronColorAndScaleSE>>("Color and scale", true);
        _representation = _application->getRender().addRepresentation<ComplexNeuronRepresentation>();
        defineOutput<std::weak_ptr<AbstractNeuronRepresentation>>("Representation", _representation);
    }

    MorphologyRepresentationNode::~MorphologyRepresentationNode()
    {
        sendOutput("Representation", std::any());
        if (auto ptr = _representation.lock()) {
            _application->getRender().removeRepresentation(ptr.get());
        }
    }

    void MorphologyRepresentationNode::renderBody()
    {
        auto ptr = _representation.lock();
        if (!ptr) {
            return;
        }

        drawProgressBar(ptr.get());

        bool wireframe = ptr->isWireframeMode();
        if (ImGui::Checkbox("Wireframe", &wireframe)) {
            ptr->setWireframeMode(wireframe);
        }
    }

    void MorphologyRepresentationNode::onInputChange(const std::string& name, const std::any& value)
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

    NodeFactory MorphologyRepresentationNode::createFactory()
    {
        return NodeFactory("Neuron morphology representation", [](ImBlueprint::Editor& editor, auto* app) {
            return editor.addNode<MorphologyRepresentationNode>(app);
        });
    }
} // namespace neoneuron
