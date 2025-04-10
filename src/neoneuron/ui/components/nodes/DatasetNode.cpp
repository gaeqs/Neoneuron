//
// Created by gaeqs on 8/04/25.
//

#include "DatasetNode.h"

#include <mindset/Dataset.h>
#include <neoneuron/structure/RepositoryView.h>

namespace neoneuron
{

    DatasetNode::DatasetNode(NeoneuronApplication* application) :
        Node("Dataset"),
        _dataset(nullptr)
    {
        defineOutput<RepositoryView>("Data");
    }

    void DatasetNode::renderBody()
    {
        ImGui::Text("Neurons: %d", _dataset->getNeuronsAmount());
        ImGui::Text("Synapses: %d", _dataset->getCircuit().getSynapses().size());
    }

    NodeFactory DatasetNode::createFactory()
    {
        return NodeFactory("Dataset",
                           [](ImBlueprint::Editor& editor, auto* app) { return editor.addNode<DatasetNode>(app); });
    }
} // namespace neoneuron
