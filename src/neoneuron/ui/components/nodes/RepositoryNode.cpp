//
// Created by gaeqs on 10/04/2025.
//

#include "RepositoryNode.h"

#include <neoneuron/structure/RepositoryView.h>

namespace neoneuron
{

    RepositoryNode::RepositoryNode(NeoneuronApplication* application) :
        Node("Repository"),
        _repository(&application->getRepository())
    {
        defineOutput<RepositoryView>("Data", RepositoryView::ofFullRepository(_repository));
    }

    NodeFactory RepositoryNode::createFactory()
    {
        return {"Repository",
                [](ImBlueprint::Editor& editor, auto* app) { return editor.addNode<RepositoryNode>(app); }};
    }
} // namespace neoneuron
