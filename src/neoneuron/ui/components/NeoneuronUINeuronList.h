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

        bool neuronSection(const mnemea::Neuron* neuron, size_t id, bool selected) const;

        void neuronList();

        void neuronInformation() const;

        void neuronNewProperty(mnemea::Neuron* neuron) const;

    public:
        explicit NeoneuronUINeuronList(NeoneuronRender* render);

        void onPreDraw() override;
    };
}


#endif //NEONEURONUINEURONLIST_H
