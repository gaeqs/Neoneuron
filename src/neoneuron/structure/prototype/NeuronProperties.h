//
// Created by gaeqs on 14/11/24.
//

#ifndef NEURONPROPERTIES_H
#define NEURONPROPERTIES_H

#include <string>

namespace neoneuron {

    enum class SegmentType : uint8_t {
        UNDEFINED = 0,
        SOMA = 1,
        AXON = 2,
        BASAL_DENDRITE = 3,
        APICAL_DENDRITE = 4,
        CUSTOM = 5,
        UNSPECIFIED_NEURITE = 6,
        GLIA_PROCESSES = 7
    };

    const std::string PROPERTY_TYPE = "type";
    const std::string PROPERTY_END = "end";
    const std::string PROPERTY_RADIUS = "radius";
    const std::string PROPERTY_PARENT = "parent";
}

#endif //NEURONPROPERTIES_H
