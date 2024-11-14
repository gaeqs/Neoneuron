//
// Created by gaeqs on 14/11/24.
//

#ifndef PROTOTYPENEURON_H
#define PROTOTYPENEURON_H

#include <string>
#include <unordered_map>
#include <vector>
#include <neoneuron/structure/Identifiable.h>

#include "PrototypeNeuronSegment.h"

namespace neoneuron {
    class PrototypeNeuron : public Identifiable {
        std::unordered_map<std::string, UID> _properties;
        std::vector<PrototypeNeuronSegment> _segments;

    public:
        explicit PrototypeNeuron(UID id);

        void defineProperty(std::string name, UID id);

        std::optional<UID> getProperty(const std::string& name) const;

        std::unordered_map<std::string, UID>& getProperties();

        const std::unordered_map<std::string, UID>& getProperties() const;

        void addSegment(PrototypeNeuronSegment segment);

        std::vector<PrototypeNeuronSegment>& getSegments();

        const std::vector<PrototypeNeuronSegment>& getSegments() const;

        void reserveSpaceForSegments(size_t space);
    };
}


#endif //PROTOTYPENEURON_H
