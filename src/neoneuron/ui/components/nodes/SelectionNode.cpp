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

#include <neoneuron/structure/RepositoryView.h>
#include <neoneuron/ui/components/nodes/SelectionNode.h>
#include <neoneuron/ui/style/MaterialSymbols.h>
#include <neoneuron/ui/style/NodeStyles.h>

namespace neoneuron
{

    void SelectionNode::sendData()
    {
        auto& neurons = _selector->getSelectedNeurons();
        auto& synapses = _selector->getSelectedSynapses();

        RepositoryView view(_repository, std::vector(neurons.begin(), neurons.end()),
                            std::vector(synapses.begin(), synapses.end()));
        sendOutput("Data", std::move(view));
        _version = _selector->getVersion();
    }

    SelectionNode::SelectionNode(NeoneuronApplication* application) :
        NeoneuronNode("Selection", ICON_MS_DATA_OBJECT, NODE_STYLE_DATA),
        _repository(&application->getRepository()),
        _selector(&application->getSelector()),
        _version(application->getSelector().getVersion())
    {
        defineOutput<RepositoryView>("Data");
        sendData();
    }

    SelectionNode::~SelectionNode()
    {
        sendOutput("Data", std::any());
    }

    void SelectionNode::renderBody()
    {
        Node::renderBody();
        if (_selector->getVersion() != _version) {
            sendData();
        }
    }

    NodeFactory SelectionNode::createFactory()
    {
        return {"Selection", [](ImBlueprint::Editor& editor, auto* app) { return editor.addNode<SelectionNode>(app); }};
    }
} // namespace neoneuron
