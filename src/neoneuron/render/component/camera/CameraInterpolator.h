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

#ifndef CAMERAINTERPOLATOR_H
#define CAMERAINTERPOLATOR_H

#include <neon/geometry/Camera.h>

#include <rush/quaternion/quat.h>
#include <rush/vector/vec.h>

namespace neoneuron
{
    class CameraInterpolator
    {
      protected:
        neon::Camera* _camera;

      public:
        CameraInterpolator(neon::Camera* camera);

        virtual ~CameraInterpolator() = default;

        [[nodiscard]] neon::Camera* getCamera() const;

        void setCamera(neon::Camera* camera);

        virtual void setPosition(rush::Vec3f position) = 0;

        virtual void teleport(rush::Vec3f position) = 0;

        virtual void setRotation(rush::Quatf rotation) = 0;

        virtual void teleportRotation(rush::Quatf rotation) = 0;

        virtual void onUpdate(float deltaTime) = 0;
    };
} // namespace neoneuron

#endif // CAMERAINTERPOLATOR_H
