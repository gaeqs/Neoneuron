//
// Created by gaeqs on 22/10/2024.
//

#include "CameraData.h"

#include <neoneuron/render/component/camera/OrbitalCameraController.h>
#include <neoneuron/render/component/camera/InstantCameraInterpolator.h>

namespace neoneuron {
    CameraData::CameraData(neon::Room* room) {
        _cameraGameObject = room->newGameObject();
        _cameraGameObject->setName("Camera controller");

        _cameraController = _cameraGameObject->newComponent<OrbitalCameraController>(
            std::make_unique<InstantCameraInterpolator>(&room->getCamera())
        );
    }

    neon::IdentifiableWrapper<CameraController> CameraData::cameraController() const {
        return _cameraController;
    }
}
