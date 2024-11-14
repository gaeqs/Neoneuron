//
// Created by gaeqs on 11/11/2024.
//

#ifndef NEONEURONUINEURONLIST_H
#define NEONEURONUINEURONLIST_H

#include <neon/Neon.h>
#include <neoneuron/render/NeoneuronRender.h>

namespace neoneuron {
    class NeoneuronUINeuronList : public neon::Component {
        NeoneuronRender* _render;

        void neuronRow(const std::vector<Neuron>& neurons, size_t row, size_t elements, float size);

        void neuronSection(const Neuron& neuron, size_t id, float size) const;

    public:
        NeoneuronUINeuronList(NeoneuronRender* render);

        void onPreDraw() override;
    };
}


#endif //NEONEURONUINEURONLIST_H
