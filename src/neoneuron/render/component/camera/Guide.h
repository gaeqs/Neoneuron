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
    };

    class Guide : public neon::Component {
        NeoneuronRender* _render;

        std::shared_ptr<neon::Model> _planeModel;
        std::shared_ptr<neon::Model> _sphereModel;
        neon::InstanceData::Instance _planeInstance;
        neon::InstanceData::Instance _sphereInstance;

        hey::Listener<bool> _positionListener;
        hey::Listener<bool> _rotationListener;

        void updatePlaneState(bool active) const;

        void updateSphereState(bool active) const;

    public:
        explicit Guide(NeoneuronRender* render);

        ~Guide() override;

        void onStart() override;
    };
}


#endif //GUIDE_H
