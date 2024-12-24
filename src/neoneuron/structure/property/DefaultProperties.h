//
// Created by gaeqs on 23/12/2024.
//

#ifndef DEFAULTPROPERTIES_H
#define DEFAULTPROPERTIES_H
#include <neoneuron/render/AbstractNeuronScene.h>
#include <neoneuron/structure/prototype/PrototypeNeuron.h>


namespace neoneuron::property {
    bool transformEditor(std::any* property, const PrototypeNeuron* neuron, const AbstractNeuronScene* scene);

    std::any transformGenerator(const PrototypeNeuron* neuron, const AbstractNeuronScene* scene);
}


#endif //DEFAULTPROPERTIES_H
