//
// Created by gaeqs on 6/01/25.
//

#ifndef NEONEURONDEFAULTS_H
#define NEONEURONDEFAULTS_H

#include <neoneuron/structure/Storage.h>
#include <neoneuron/structure/property/DefinedProperty.h>

namespace neoneuron
{

    const std::string PROPERTY_LOD = "neoneuron:lod";

    void initDefaultProperties(Storage<DefinedProperty>& storage);

} // namespace neoneuron

#endif //NEONEURONDEFAULTS_H
