//
// Created by gaeqs on 6/01/25.
//

#ifndef NEONEURONDEFAULTS_H
#define NEONEURONDEFAULTS_H

#include <neoneuron/loader/Loader.h>
#include <neoneuron/structure/Storage.h>
#include <neoneuron/structure/property/DefinedProperty.h>

namespace neoneuron {
    void initDefaultProperties(Storage<DefinedProperty>& storage);

    void initDefaultLoaders(Storage<LoaderBuilder>& storage);
}


#endif //NEONEURONDEFAULTS_H
