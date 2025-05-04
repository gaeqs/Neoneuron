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

#include "CameraNode.h"

#include "neoneuron/render/component/camera/InstantCameraInterpolator.h"
#include "neoneuron/render/component/camera/OrbitalCameraController.h"

namespace
{
    constexpr float DEFAULT_FRUSTUM_NEAR = 0.1f;
    constexpr float DEFAULT_FRUSTUM_FAR = 10000.0f;
    constexpr float DEFAULT_FRUSTUM_FOV = 1.7453f; // RADIANS
} // namespace

namespace neoneuron
{

    CameraNode::CameraNode(NeoneuronApplication* application) :
        Node("Camera"),
        _camera(createDefaultCamera()),
        _holder(application->getRender().getRoom()->newGameObject()),
        _controller(createDefaultCameraController(_holder, _camera.get()))
    {
        _holder->setName("Camera Node GO");
        defineOutput<neon::IdentifiableWrapper<CameraController>>("Camera", _controller);
    }

    CameraNode::~CameraNode()
    {
        sendOutput("Camera", std::any());
        _camera.release();
        _controller->destroy();
        _holder->destroy();
    }

    void CameraNode::renderBody()
    {
        ImGui::Text("Camera: (%f, %f, %f)", _camera->getPosition().x(), _camera->getPosition().y(),
                    _camera->getPosition().z());

        auto lookAt = _camera->getForward();
        ImGui::Text("LookAt: (%f, %f, %f)", lookAt.x(), lookAt.y(), lookAt.z());
    }

    NodeFactory CameraNode::createFactory()
    {
        return NodeFactory("Camera",
                           [](ImBlueprint::Editor& editor, auto* app) { return editor.addNode<CameraNode>(app); });
    }

    std::unique_ptr<neon::Camera> CameraNode::createDefaultCamera()
    {
        return std::make_unique<neon::Camera>(
            neon::Frustum(DEFAULT_FRUSTUM_NEAR, DEFAULT_FRUSTUM_FAR, 1.0f, DEFAULT_FRUSTUM_FOV));
    }

    neon::IdentifiableWrapper<CameraController> CameraNode::createDefaultCameraController(
        const neon::IdentifiableWrapper<neon::GameObject>& holder, neon::Camera* camera)
    {
        return holder->newComponent<OrbitalCameraController>(std::make_unique<InstantCameraInterpolator>(camera));
    }

} // namespace neoneuron