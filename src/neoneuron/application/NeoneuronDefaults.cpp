//
// Created by gaeqs on 6/01/25.
//

#include "NeoneuronDefaults.h"

#include <neoneuron/loader/SWCLoader.h>
#include <neoneuron/loader/XMLLoader.h>
#include <neoneuron/structure/property/DefaultProperties.h>
#include <neoneuron/structure/prototype/NeuronProperties.h>

namespace neoneuron {
    void initDefaultProperties(Storage<DefinedProperty>& storage) {
        storage.add(
            PROPERTY_TRANSFORM,
            {
                PROPERTY_TRANSFORM,
                typeid(NeuronTransform),
                PropertyTarget::NEURON,
                "Transform",
                property::transformEditor,
                property::transformGenerator
            }
        );
        storage.add(
            PROPERTY_NAME,
            {
                PROPERTY_NAME,
                typeid(std::string),
                PropertyTarget::NEURON,
                "Name",
                property::nameEditor,
                property::nameGenerator
            }
        );
    }

    void initDefaultLoaders(Storage<LoaderBuilder>& storage) {
        storage.add(
            XML_LOADER_NAME,
            LoaderBuilder(
                XML_LOADER_NAME,
                XML_LOADER_DISPLAY_NAME,
                [](auto* fileSystem, auto& file) {
                    return std::make_unique<XMLLoader>(fileSystem, file);
                },
                true
            )
        );

        storage.add(
            SWC_LOADER_NAME,
            LoaderBuilder(
                SWC_LOADER_NAME,
                SWC_LOADER_DISPLAY_NAME,
                [](auto*, auto& file) {
                    return std::make_unique<SWCLoader>(file);
                },
                false
            )
        );
    }
}
