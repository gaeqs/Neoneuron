//
// Created by gaelr on 04/11/2022.
//

#ifndef NEON_GLOBALPARAMETERSUPDATERCOMPONENT_H
#define NEON_GLOBALPARAMETERSUPDATERCOMPONENT_H

#include <neon/Neon.h>

namespace neoneuron {
    class NeoneuronRender;

    struct Matrices {
        rush::Mat4f view;
        rush::Mat4f projectionView;
        rush::Mat4f inverseProjection;
        float near;
        float far;
    };

    struct Time {
        float currentTime;
    };

    struct Scene {
        rush::Vec3f sceneCenter;
        float dummy;
        rush::Vec3f sceneRadius;
    };

    class GlobalParametersUpdaterComponent : public neon::Component {
        NeoneuronRender& _render;

    public:
        explicit GlobalParametersUpdaterComponent(NeoneuronRender& render);

        ~GlobalParametersUpdaterComponent() override;

        void onStart() override;

        void onUpdate(float deltaTime) override;
    };

    REGISTER_COMPONENT(GlobalParametersUpdaterComponent, "Global Parameters Updater")
}

#endif //NEON_GLOBALPARAMETERSUPDATERCOMPONENT_H
