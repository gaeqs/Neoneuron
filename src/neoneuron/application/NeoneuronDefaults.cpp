//
// Created by gaeqs on 6/01/25.
//

#include "NeoneuronDefaults.h"

#include <mindset/util/NeuronTransform.h>

#include <neoneuron/structure/property/DefaultProperties.h>
#include <mindset/DefaultProperties.h>

namespace neoneuron
{
    void initDefaultProperties(Storage<DefinedProperty>& storage)
    {
        storage.add(mindset::PROPERTY_TRANSFORM,
                    DefinedProperty(mindset::PROPERTY_TRANSFORM, typeid(mindset::NeuronTransform),
                                    PropertyTarget::NEURON, "Transform", property::transformEditor,
                                    property::transformGenerator, property::transformToJson,
                                    property::transformFromJson));

        storage.add(mindset::PROPERTY_NAME,
                    DefinedProperty{mindset::PROPERTY_NAME, typeid(std::string), PropertyTarget::NEURON, "Name",
                                    property::nameEditor, property::nameGenerator, property::nameToJson,
                                    property::nameFromJson});

        storage.add(PROPERTY_LOD, DefinedProperty{PROPERTY_LOD, typeid(uint32_t), PropertyTarget::NEURON,
                                                  "Level of Detail", property::lodEditor, property::lodGenerator,
                                                  property::uint32ToJson, property::uint32FromJson});

        storage.add(mindset::PROPERTY_POSITION,
                    DefinedProperty{mindset::PROPERTY_POSITION, typeid(rush::Vec3f), PropertyTarget::SEGMENT, "End",
                                    nullptr, nullptr, property::vec3fToJson, property::vec3fFromJson});

        storage.add(mindset::PROPERTY_RADIUS,
                    DefinedProperty{mindset::PROPERTY_RADIUS, typeid(float), PropertyTarget::SEGMENT, "Radius", nullptr,
                                    nullptr, property::floatToJson, property::floatFromJson});

        storage.add(mindset::PROPERTY_PARENT,
                    DefinedProperty{mindset::PROPERTY_PARENT, typeid(int64_t), PropertyTarget::SEGMENT, "Parent",
                                    nullptr, nullptr, property::int64ToJson, property::int64FromJson});

        storage.add(mindset::PROPERTY_NEURITE_TYPE,
                    DefinedProperty{mindset::PROPERTY_NEURITE_TYPE, typeid(int64_t), PropertyTarget::SEGMENT, "Type",
                                    nullptr, nullptr, property::uint32ToJson, property::uint32FromJson});
    }
} // namespace neoneuron
