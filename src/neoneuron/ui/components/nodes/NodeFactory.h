//
// Created by gaeqs on 8/04/25.
//

#ifndef NODEFACTORY_H
#define NODEFACTORY_H

#include <functional>
#include <imblueprint/imblueprint.h>
#include <neoneuron/application/NeoneuronApplication.h>

namespace neoneuron
{

    class NodeFactory
    {
        std::string _displayName;
        std::function<ImBlueprint::Node*(ImBlueprint::Editor&, NeoneuronApplication*)> _creator;

      public:
        NodeFactory(std::string displayName,
                    std::function<ImBlueprint::Node*(ImBlueprint::Editor&, NeoneuronApplication*)> creator);

        [[nodiscard]] std::string getDisplayName() const;

        ImBlueprint::Node* create(NeoneuronApplication* application, ImBlueprint::Editor& editor) const;
    };

} // namespace neoneuron

#endif // NODEFACTORY_H
