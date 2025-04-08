//
// Created by gaeqs on 8/04/25.
//

#include "DatasetNode.h"

#include <mindset/Dataset.h>

namespace neoneuron
{

    DatasetNode::DatasetNode(NeoneuronApplication* application) :
        Node("Dataset"),
        _dataset(&application->getDataset())
    {
        defineOutput<mindset::Dataset*>("Dataset", _dataset);
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
