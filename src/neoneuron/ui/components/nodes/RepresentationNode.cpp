//
// Created by gaeqs on 12/04/2025.
//

#include "RepresentationNode.h"

#include <neoneuron/structure/RepositoryView.h>

namespace neoneuron
{

    RepresentationNode::RepresentationNode(NeoneuronApplication* application) :
        Node("Representation"),
        _application(application)
    {
        defineInput<RepositoryView>("Data");
    }

    void RepresentationNode::onInputChange(const std::string& name, const std::any& value)
    {
        auto* set = std::any_cast<const RepositoryView>(&value);
        if (set != nullptr) {
            for (auto representation : _application->getRender().getRepresentations()) {
                representation->refreshData(*set);
            }
        } else {
            for (auto representation : _application->getRender().getRepresentations()) {
                representation->clearData();
            }
        }
    }

    NodeFactory RepresentationNode::createFactory()
    {
        return NodeFactory(
            "Representation", [](ImBlueprint::Editor& editor, auto* app) { return editor.addNode<RepresentationNode>(app); });
    }
} // namespace neoneuron
