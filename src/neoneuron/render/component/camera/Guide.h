// Copyright (c) 2025. VG-Lab/URJC.
//
// Authors: Gael Rial Costas <gael.rial.costas@urjc.es>
//
// This file is part of Neoneuron <gitlab.gmrv.es/g.rial/neoneuron>
//
// This library is free software; you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License version 3.0 as published
// by the Free Software Foundation.
//
// This library is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this library; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

#ifndef GUIDE_H
#define GUIDE_H

#include <hey/Listener.h>
#include <neon/Neon.h>

namespace neoneuron
{
    class NeoneuronRender;
    class OrbitalCameraController;
    class Viewport;

    class PlaneGuide : public neon::Component
    {
        struct GuideInstancingData
        {
            float active;
            float lastUpdate;
        };

        Viewport* _viewport;

        std::shared_ptr<neon::Model> _planeModel;
        neon::InstanceData::Instance _planeInstance;

        hey::Listener<bool> _positionListener;

        void updatePlaneState(bool active) const;

      public:
        explicit PlaneGuide(Viewport* viewport);

        ~PlaneGuide() override;

        void onStart() override;
    };

    class SphereGuide : public neon::Component
    {
        struct GuideInstancingData
        {
            float active;
            float lastUpdate;
            rush::Vec3f center;
            float radius;
        };

        Viewport* _viewport;
        neon::IdentifiableWrapper<OrbitalCameraController> _orbitalController;

        std::shared_ptr<neon::Model> _sphereModel;
        neon::InstanceData::Instance _sphereInstance;
        hey::Listener<bool> _rotationListener;

        bool _state;
        float _lastUpdate;

        void updateSphereState() const;

      public:
        SphereGuide(Viewport* viewport, neon::IdentifiableWrapper<OrbitalCameraController> controller);

        ~SphereGuide() override;

        void onStart() override;

        void onPreDraw() override;
    };

    class PointGuide : public neon::Component
    {
        struct GuideInstancingData
        {
            float active;
            float lastUpdate;
            rush::Vec3f center;
        };

        Viewport* _viewport;
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
        PointGuide(Viewport* viewport, neon::IdentifiableWrapper<OrbitalCameraController> controller);

        ~PointGuide() override;

        void onStart() override;

        void onPreDraw() override;
    };
} // namespace neoneuron

#endif // GUIDE_H
