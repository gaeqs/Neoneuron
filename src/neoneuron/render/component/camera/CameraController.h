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

#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#include <neon/Neon.h>

#include <neoneuron/render/Viewport.h>
#include <neoneuron/render/component/camera/CameraInterpolator.h>

namespace neoneuron
{
    class CameraData;

    class CameraController : public neon::Component
    {
        std::unordered_map<Viewport*, std::vector<neon::IdentifiableWrapper<Component>>> _viewports;

      public:
        explicit CameraController() = default;

        ~CameraController() override;

        void registerViewport(Viewport* viewport);

        void unregisterViewport(Viewport* viewport);

        bool isHovered() const;

        void sendActivePosition(bool active);

        void sendActiveRotation(bool active);

        virtual neon::Camera* getCamera() const = 0;

        virtual void focusOn(const rush::AABB<3, float>& aabb) = 0;

        virtual void forcePosition(rush::Vec3f position) = 0;

        virtual void setInterpolator(std::unique_ptr<CameraInterpolator> interpolator) = 0;

        virtual std::vector<neon::IdentifiableWrapper<Component>> onViewportRegister(Viewport* viewport) = 0;
    };
} // namespace neoneuron

#endif // CAMERACONTROLLER_H
