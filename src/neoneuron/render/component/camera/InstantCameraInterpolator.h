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

#ifndef INSTANTCAMERAINTERPOLATOR_H
#define INSTANTCAMERAINTERPOLATOR_H

#include <neon/Neon.h>

#include <neoneuron/render/component/camera/CameraInterpolator.h>

namespace neoneuron
{
    class InstantCameraInterpolator : public CameraInterpolator
    {
      public:
        explicit InstantCameraInterpolator(neon::Camera* camera);

        void setPosition(rush::Vec3f position) override;

        void teleport(rush::Vec3f position) override;

        void setRotation(rush::Quatf rotation) override;

        void teleportRotation(rush::Quatf rotation) override;

        void onUpdate(float deltaTime) override;
    };
} // namespace neoneuron

#endif //INSTANTCAMERAINTERPOLATOR_H
