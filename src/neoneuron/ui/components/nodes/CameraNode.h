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

#ifndef CAMERANODE_H
#define CAMERANODE_H

#include <neon/geometry/Camera.h>
#include <neoneuron/ui/components/nodes/NeoneuronNode.h>
#include <neoneuron/render/component/camera/CameraController.h>

#include "NodeFactory.h"

namespace neoneuron
{
    class CameraNode : public NeoneuronNode
    {
        std::unique_ptr<neon::Camera> _camera;
        neon::IdentifiableWrapper<neon::GameObject> _holder;
        neon::IdentifiableWrapper<CameraController> _controller;

      public:
        explicit CameraNode(NeoneuronApplication* application);

        ~CameraNode() override;

        void renderBody() override;

        static NodeFactory createFactory();

        static std::unique_ptr<neon::Camera> createDefaultCamera();

        static neon::IdentifiableWrapper<CameraController> createDefaultCameraController(
            const neon::IdentifiableWrapper<neon::GameObject>& holder, neon::Camera* camera);
    };
} // namespace neoneuron

#endif // CAMERANODE_H
