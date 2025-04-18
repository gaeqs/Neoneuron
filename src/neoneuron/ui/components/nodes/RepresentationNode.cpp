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
        defineInput<RepositoryView>("Data", true);
        _representation = _application->getRender().addRepresentation<ComplexNeuronRepresentation>();
        defineOutput<AbstractNeuronRepresentation*>("Representation", _representation);
    }

    RepresentationNode::~RepresentationNode()
    {
        _application->getRender().removeRepresentation(_representation);
    }

    void RepresentationNode::onInputChange(const std::string& name, const std::any& value)
    {
        auto data = getMultipleInputs<RepositoryView>("Data");
        if (!data.has_value()) {
            _representation->clearData();
            return;
        }

        auto combined = RepositoryView::combine(data.value());
        if (combined.has_value()) {
            _representation->refreshData(combined.value());
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
