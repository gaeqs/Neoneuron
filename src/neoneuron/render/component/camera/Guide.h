//
// Created by gaeqs on 23/10/2024.
//

#ifndef GUIDE_H
#define GUIDE_H

#include <hey/Listener.h>
#include <neon/Neon.h>

namespace neoneuron {
    class NeoneuronRender;

    struct GuideInstancingData {
        float active;
        float lastUpdate;

        static neon::InputDescription getInstancingDescription() {
            neon::InputDescription description(
                sizeof(GuideInstancingData),
                neon::InputRate::INSTANCE
            );
            description.addAttribute(1, 0);
            description.addAttribute(1, 4);

            return description;
        }
    };

    class Guide : public neon::Component {
        NeoneuronRender* _render;

        std::shared_ptr<neon::Model> _model;
        neon::InstanceData::Instance _instance;
        hey::Listener<bool> _positionListener;

        void updateState (bool active);

    public:
        explicit Guide(NeoneuronRender* render);

        ~Guide() override;

        void onStart() override;
    };
}


#endif //GUIDE_H
