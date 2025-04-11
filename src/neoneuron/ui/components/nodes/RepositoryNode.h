//
// Created by gaeqs on 10/04/2025.
//

#ifndef REPOSITORYNODE_H
#define REPOSITORYNODE_H

#include "NodeFactory.h"

#include <imblueprint/imblueprint.h>

#include <neoneuron/structure/Repository.h>
#include <neoneuron/application/NeoneuronApplication.h>

namespace neoneuron
{

    class RepositoryNode : public ImBlueprint::Node
    {
        Repository* _repository;
        uint64_t _version;

      public:
        explicit RepositoryNode(NeoneuronApplication* application);

        ~RepositoryNode() override = default;

        void renderBody() override;

        static NodeFactory createFactory();
    };

} // namespace neoneuron

#endif // REPOSITORYNODE_H
