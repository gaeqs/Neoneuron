//
// Created by gaelr on 04/11/2022.
//

#ifndef NEON_GLOBALPARAMETERSUPDATERCOMPONENT_H
#define NEON_GLOBALPARAMETERSUPDATERCOMPONENT_H

#include <array>
#include <neon/Neon.h>

namespace neoneuron
{
    class NeoneuronRender;

    struct Matrices
    {
        rush::Mat4f view;
        rush::Mat4f projectionView;
        rush::Mat4f inverseProjection;
        std::array<rush::Plane<float>, 6> planes;
        float near;
        float far;
    };

    struct Scene
    {
        rush::Vec3f sceneCenter;
        float dummy;
        rush::Vec3f sceneRadius;
        uint32_t sectionsAmount;
        uint32_t jointsAmount;
    };

    class GlobalParametersUpdaterComponent : public neon::Component
    {
        NeoneuronRender& _render;

      public:
        explicit GlobalParametersUpdaterComponent(NeoneuronRender& render);

        ~GlobalParametersUpdaterComponent() override;

        void onStart() override;

        void onPreDraw() override;
    };

    REGISTER_COMPONENT(GlobalParametersUpdaterComponent, "Global Parameters Updater")
} // namespace neoneuron

#endif //NEON_GLOBALPARAMETERSUPDATERCOMPONENT_H
