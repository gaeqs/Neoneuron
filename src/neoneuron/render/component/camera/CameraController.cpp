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

#include "CameraController.h"

namespace neoneuron
{
    CameraController::~CameraController()
    {
        for (auto& collection : _viewports | std::views::values) {
            for (auto& component : collection) {
                if (component.isValid()) {
                    component->destroy();
                }
            }
        }
    }

    void CameraController::registerViewport(Viewport* viewport)
    {
        if (_viewports.contains(viewport)) {
            return;
        }
        _viewports.insert({viewport, onViewportRegister(viewport)});
    }

    void CameraController::unregisterViewport(Viewport* viewport)
    {
        auto it = _viewports.find(viewport);
        if (it == _viewports.end()) {
            return;
        }

        for (const auto& component : it->second) {
            if (component.isValid()) {
                component->destroy();
            }
        }

        _viewports.erase(it);
    }

    bool CameraController::isHovered() const
    {
        return std::ranges::any_of(_viewports, [](const auto& vp) { return vp.first->isHovered(); });
    }

    void CameraController::sendActivePosition(bool active)
    {
        for (const auto& vp : _viewports | std::views::keys) {
            if (auto& pos = vp->getCameraData().onActivePosition(); pos.getValue() != active) {
                pos.setValue(active);
            }
        }
    }

    void CameraController::sendActiveRotation(bool active)
    {
        for (const auto& vp : _viewports | std::views::keys) {
            if (auto& rot = vp->getCameraData().onActiveRotation(); rot.getValue() != active) {
                rot.setValue(active);
            }
        }
    }
} // namespace neoneuron
