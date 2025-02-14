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
                property::transformGenerator,
                property::transformToJson,
                property::transformFromJson
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
                property::nameGenerator,
                property::nameToJson,
                property::nameFromJson
            }
        );

        storage.add(
            PROPERTY_END,
            {
                PROPERTY_END,
                typeid(rush::Vec3f),
                PropertyTarget::SEGMENT,
                "End",
                nullptr,
                nullptr,
                property::vec3fToJson,
                property::vec3fFromJson
            }
        );

        storage.add(
            PROPERTY_RADIUS,
            {
                PROPERTY_RADIUS,
                typeid(float),
                PropertyTarget::SEGMENT,
                "Radius",
                nullptr,
                nullptr,
                property::floatToJson,
                property::floatFromJson
            }
        );

        storage.add(
            PROPERTY_PARENT,
            {
                PROPERTY_PARENT,
                typeid(int64_t),
                PropertyTarget::SEGMENT,
                "Parent",
                nullptr,
                nullptr,
                property::int64ToJson,
                property::int64FromJson
            }
        );

        storage.add(
            PROPERTY_TYPE,
            {
                PROPERTY_TYPE,
                typeid(int64_t),
                PropertyTarget::SEGMENT,
                "Type",
                nullptr,
                nullptr,
                property::uint32ToJson,
                property::uint32FromJson
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
