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

#ifndef ORBITALCAMERACONTROLLER_H
#define ORBITALCAMERACONTROLLER_H

#include <neoneuron/render/Viewport.h>
#include <neoneuron/render/component/camera/CameraController.h>

namespace neoneuron
{
    class NeoneuronRender;

    class OrbitalCameraController : public CameraController
    {
        struct Position
        {
            rush::Vec3f position;
            rush::Vec2f rotation;
            float radius;
        };

        std::unique_ptr<CameraInterpolator> _interpolator;

        Position _position;

        rush::Vec3f _velocity;
        rush::Vec2f _angularVelocity;
        float _radiusVelocity;

        float _radiusScale;
        float _positionScale;

        bool _dragPosition;
        bool _dragRotation;

        void sendPosition() const;

        void dragPosition(const neon::CursorMoveEvent& event);

        void dragRotation(const neon::CursorMoveEvent& event);

        bool updatePosition(float deltaTime);

        bool updateRotation(float deltaTime);

        bool updateRadius(float deltaTime);

      public:
        OrbitalCameraController(std::unique_ptr<CameraInterpolator> interpolator);

        ~OrbitalCameraController() override = default;

        neon::Camera* getCamera() const override;

        void focusOn(const rush::AABB<3, float>& aabb) override;

        void forcePosition(rush::Vec3f position) override;

        void setInterpolator(std::unique_ptr<CameraInterpolator> interpolator) override;

        std::vector<neon::IdentifiableWrapper<Component>> onViewportRegister(Viewport* viewport) override;

        void onMouseButton(const neon::MouseButtonEvent& event) override;

        void onCursorMove(const neon::CursorMoveEvent& event) override;

        void onScroll(const neon::ScrollEvent& event) override;

        void onStart() override;

        void onUpdate(float deltaTime) override;

        float getRadius() const;

        [[nodiscard]] rush::Vec3f getCenter() const;

        [[nodiscard]] rush::Vec3f getFinalPosition() const;

        void setCenterKeepPosition(rush::Vec3f center);
    };
} // namespace neoneuron

#endif // ORBITALCAMERACONTROLLER_H
