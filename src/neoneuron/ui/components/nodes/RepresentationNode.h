//
// Created by gaeqs on 12/04/2025.
//

#ifndef REPRESENTATIONNODE_H
#define REPRESENTATIONNODE_H


#include "NodeFactory.h"

#include <imblueprint/imblueprint.h>
#include <neoneuron/application/NeoneuronApplication.h>
#include <neoneuron/render/complex/ComplexNeuronRepresentation.h>

namespace neoneuron
{

    class RepresentationNode : public ImBlueprint::Node
    {
        NeoneuronApplication* _application;
        ComplexNeuronRepresentation* _representation;

    public:
        explicit RepresentationNode(NeoneuronApplication* application);

        ~RepresentationNode() override;

        void onInputChange(const std::string& name, const std::any& value) override;

        static NodeFactory createFactory();

    };

} // namespace neoneuron



#endif //REPRESENTATIONNODE_H
