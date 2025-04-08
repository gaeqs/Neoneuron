//
// Created by gaeqs on 8/04/25.
//

#ifndef DATASETNODE_H
#define DATASETNODE_H

#include "NodeFactory.h"

#include <imblueprint/imblueprint.h>
#include <neoneuron/application/NeoneuronApplication.h>

namespace neoneuron
{

    class DatasetNode : public ImBlueprint::Node
    {

        mindset::Dataset* _dataset;

    public:

        DatasetNode(NeoneuronApplication* application);

        ~DatasetNode() override = default;

        void renderBody() override;


        static NodeFactory createFactory();

    };

} // namespace neoneuron

#endif // DATASETNODE_H
