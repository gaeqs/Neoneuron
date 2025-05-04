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

#include "GlobalParametersUpdaterComponent.h"

#include <neoneuron/render/NeoneuronRender.h>

#include "camera/OrbitalCameraController.h"

namespace neoneuron
{
    GlobalParametersUpdaterComponent::GlobalParametersUpdaterComponent(NeoneuronRender& render) :
        _render(render)
    {
    }

    GlobalParametersUpdaterComponent::~GlobalParametersUpdaterComponent() = default;

    void GlobalParametersUpdaterComponent::onStart()
    {
    }

    void GlobalParametersUpdaterComponent::onPreDraw()
    {
        auto& camera = getRoom()->getCamera();

        auto& buffer = getApplication()->getRender()->getGlobalUniformBuffer();

        buffer.uploadData<Matrices>(0, Matrices{
                                           camera.getView(),
                                           camera.getViewProjection(),
                                           camera.getFrustum().getInverseProjection(),
                                           camera.getPlanes(),
                                           camera.getFrustum().getNear(),
                                           camera.getFrustum().getFar(),
                                       });

        auto& renderData = _render.getRenderData();
        renderData.currentTime = _render.getCurrentTime();
        renderData.frame = _render.getApplication().getCurrentFrameInformation().currentFrame;
        buffer.uploadData<NeoneuronRenderData>(1, renderData);

        auto bb = _render.getCombinedAABB();

        buffer.uploadData<Scene>(2, Scene{bb.center, 0, bb.radius});
    }
} // namespace neoneuron
