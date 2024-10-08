//
// Created by gaeqs on 8/10/24.
//

#ifndef NEURON_H
#define NEURON_H

#include <vector>
#include <neoneuron/structure/Identifiable.h>

#include <neoneuron/structure/NeuronSegment.h>

namespace neoneuron {
    class Neuron : public Identifiable {
        std::vector<NeuronSegment> _segments;

    public:
        Neuron();

        Neuron(UID uid, const std::vector<NeuronSegment>& segments);

        Neuron(UID uid, std::vector<NeuronSegment>&& segments);

        [[nodiscard]] const std::vector<NeuronSegment>& getSegments() const;
    };
}

#endif //NEURON_H
