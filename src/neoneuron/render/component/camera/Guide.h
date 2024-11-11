//
// Created by gaeqs on 23/10/2024.
//

#ifndef GUIDE_H
#define GUIDE_H

#include <hey/Listener.h>
#include <neon/Neon.h>

namespace neoneuron {
    class NeoneuronRender;
    class OrbitalCameraController;

    class PlaneGuide : public neon::Component {
        struct GuideInstancingData {
            float active;
            float lastUpdate;
        };

        NeoneuronRender* _render;

        std::shared_ptr<neon::Model> _planeModel;
        neon::InstanceData::Instance _planeInstance;

        hey::Listener<bool> _positionListener;

        void updatePlaneState(bool active) const;

    public:
        explicit PlaneGuide(NeoneuronRender* render);

        ~PlaneGuide() override;

        void onStart() override;
    };

    class SphereGuide : public neon::Component {
        struct GuideInstancingData {
            float active;
            float lastUpdate;
            rush::Vec3f center;
            float radius;
        };

        NeoneuronRender* _render;
        neon::IdentifiableWrapper<OrbitalCameraController> _orbitalController;

        std::shared_ptr<neon::Model> _sphereModel;
        neon::InstanceData::Instance _sphereInstance;
        hey::Listener<bool> _rotationListener;

        bool _state;
        float _lastUpdate;

        void updateSphereState() const;

    public:
        explicit SphereGuide(NeoneuronRender* render, neon::IdentifiableWrapper<OrbitalCameraController> controller);

        ~SphereGuide() override;

        void onStart() override;

        void onPreDraw() override;
    };

    class PointGuide : public neon::Component {
        struct GuideInstancingData {
            float active;
            float lastUpdate;
            rush::Vec3f center;
        };

        NeoneuronRender* _render;
        neon::IdentifiableWrapper<OrbitalCameraController> _orbitalController;

        std::shared_ptr<neon::Model> _sphereModel;
        neon::InstanceData::Instance _sphereInstance;
        hey::Listener<bool> _positionListener;
        hey::Listener<bool> _rotationListener;

        bool _positionState;
        bool _rotationState;
        bool _state;
        float _lastUpdate;

        void updatePointState();

    public:
        explicit PointGuide(NeoneuronRender* render, neon::IdentifiableWrapper<OrbitalCameraController> controller);

        ~PointGuide() override;

        void onStart() override;

        void onPreDraw() override;
    };
}


#endif //GUIDE_H
