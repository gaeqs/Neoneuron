//
// Created by gaeqs on 8/04/25.
//

#include "NodeFactory.h"

namespace neoneuron
{

    NodeFactory::NodeFactory(std::string displayName,
                             std::function<ImBlueprint::Node*(ImBlueprint::Editor&, NeoneuronApplication*)> creator) :
        _displayName(std::move(displayName)),
        _creator(std::move(creator))
    {
    }

    std::string NodeFactory::getDisplayName() const
    {
        return _displayName;
    }

    ImBlueprint::Node* NodeFactory::create(NeoneuronApplication* application, ImBlueprint::Editor& editor) const
    {
        if (_creator != nullptr) {
            return _creator(editor, application);
        }
        return nullptr;
    }
} // namespace neoneuron
