//
// Created by gaeqs on 12/04/2025.
//

#include "RepresentationNode.h"

#include <neoneuron/render/complex/ComplexNeuronRepresentation.h>
#include <neoneuron/structure/RepositoryView.h>

namespace neoneuron
{

    RepresentationNode::RepresentationNode(NeoneuronApplication* application) :
        Node("Representation"),
        _application(application)
    {
        defineInput<RepositoryView>("Data", false);
        _representation = _application->getRender().addRepresentation<ComplexNeuronRepresentation>();
    }

    RepresentationNode::~RepresentationNode()
    {
        _application->getRender().removeRepresentation(_representation);
    }

    void RepresentationNode::onInputChange(const std::string& name, const std::any& value)
    {
        auto* set = std::any_cast<const RepositoryView>(&value);
        if (set != nullptr) {
            _representation->refreshData(*set);
        } else {
            _representation->clearData();
        }
    }

    NodeFactory RepresentationNode::createFactory()
    {
        return NodeFactory("Representation", [](ImBlueprint::Editor& editor, auto* app) {
            return editor.addNode<RepresentationNode>(app);
        });
    }
} // namespace neoneuron
