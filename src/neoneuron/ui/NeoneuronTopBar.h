//
// Created by gaeqs on 11/10/24.
//

#ifndef NEONEURONTOPBAR_H
#define NEONEURONTOPBAR_H

#include <neon/Neon.h>

namespace neoneuron {
    class NeoneuronRender;

    class NeoneuronTopBar : public neon::Component {
        NeoneuronRender* _render;

        void openFile() const;

    public:
        explicit NeoneuronTopBar(NeoneuronRender* render);

        void onStart() override;

        void onPreDraw() override;
    };
}


#endif //NEONEURONTOPBAR_H
