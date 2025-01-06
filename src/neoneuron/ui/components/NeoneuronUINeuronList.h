//
// Created by gaeqs on 11/11/2024.
//

#ifndef NEONEURONUINEURONLIST_H
#define NEONEURONUINEURONLIST_H

#include <neon/Neon.h>
#include <neoneuron/render/NeoneuronRender.h>
#include <neoneuron/structure/simple/SimpleNeuron.h>

namespace neoneuron {
    class NeoneuronUINeuronList : public neon::Component {
        NeoneuronRender* _render;
        std::optional<UID> _selectedNeuron;

        bool neuronSection(const PrototypeNeuron& neuron, size_t id);

        void neuronList();

        void neuronInformation();

        void neuronNewProperty(PrototypeNeuron* neuron) const;

    public:
        explicit NeoneuronUINeuronList(NeoneuronRender* render);

        void onPreDraw() override;
    };
}


#endif //NEONEURONUINEURONLIST_H
