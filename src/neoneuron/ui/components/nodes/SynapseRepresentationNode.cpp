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

#include "SynapseRepresentationNode.h"

#include <neoneuron/render/synapse/SynapseRepresentation.h>
#include <neoneuron/structure/RepositoryView.h>

namespace neoneuron
{

    SynapseRepresentationNode::SynapseRepresentationNode(NeoneuronApplication* application) :
        Node("Synapse Representation"),
        _application(application)
    {
        defineInput<RepositoryView>("Data", true);
        defineInput<Viewport*>("Viewport", true);
        _representation = _application->getRender().addRepresentation<SynapseRepresentation>();
        defineOutput<std::weak_ptr<AbstractNeuronRepresentation>>("Representation", _representation);
    }

    SynapseRepresentationNode::~SynapseRepresentationNode()
    {
        sendOutput("Representation", std::any());
        if (auto ptr = _representation.lock()) {
            _application->getRender().removeRepresentation(ptr.get());
        }
    }

    void SynapseRepresentationNode::onInputChange(const std::string& name, const std::any& value)
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

            std::unordered_set set(vec.begin(), vec.end());
            ptr->setViewports(set);
        }
    }

    NodeFactory SynapseRepresentationNode::createFactory()
    {
        return NodeFactory("Synapse Representation", [](ImBlueprint::Editor& editor, auto* app) {
            return editor.addNode<SynapseRepresentationNode>(app);
        });
    }
} // namespace neoneuron
