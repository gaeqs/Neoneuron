//
// Created by gaeqs on 10/04/2025.
//

#include "RepositoryNode.h"

#include <neoneuron/structure/RepositoryView.h>

namespace neoneuron
{

    RepositoryNode::RepositoryNode(NeoneuronApplication* application) :
        Node("Repository"),
        _repository(&application->getRepository()),
        _version(application->getRepository().getVersion())
    {
        defineOutput<RepositoryView>("Data", RepositoryView::ofFullRepository(_repository));
    }

    void RepositoryNode::renderBody()
    {
        Node::renderBody();
        if (_repository->getVersion() != _version) {
            sendOutput("Data", RepositoryView::ofFullRepository(_repository));
            _version = _repository->getVersion();
        }
    }

    NodeFactory RepositoryNode::createFactory()
    {
        return {"Repository",
                [](ImBlueprint::Editor& editor, auto* app) { return editor.addNode<RepositoryNode>(app); }};
    }
} // namespace neoneuron
