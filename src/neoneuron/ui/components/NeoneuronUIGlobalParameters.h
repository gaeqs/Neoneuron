//
// Created by gaeqs on 29/11/24.
//

#ifndef NEONEURONUIGLOBALPARAMETERS_H
#define NEONEURONUIGLOBALPARAMETERS_H

#include <neon/Neon.h>
#include <neoneuron/render/NeoneuronRender.h>

namespace neoneuron
{
    class NeoneuronUIGlobalParameters : public neon::Component
    {
        NeoneuronRender* _render;

      public:
        NeoneuronUIGlobalParameters(NeoneuronRender* render);

        void onPreDraw() override;
    };
} // namespace neoneuron

#endif //NEONEURONUIGLOBALPARAMETERS_H
