//
// Created by gaeqs on 22/10/2024.
//

#include "CameraData.h"

#include <neoneuron/render/NeoneuronRender.h>
#include <neoneuron/render/component/camera/OrbitalCameraController.h>
#include <neoneuron/render/component/camera/InstantCameraInterpolator.h>

#include <neoneuron/render/component/camera/Guide.h>

namespace neoneuron
{
    CameraData::~CameraData()
    {
        if (_cameraGameObject.isValid()) {
            _cameraGameObject->destroy();
        }
    }

    CameraData::CameraData(NeoneuronRender* render)
    {
        auto* room = render->getRoom().get();
        _cameraGameObject = room->newGameObject();
        _cameraGameObject->setName("Camera");

        _cameraController = _cameraGameObject->newComponent<OrbitalCameraController>(
            render, this, std::make_unique<InstantCameraInterpolator>(&room->getCamera()));
    }

    neon::IdentifiableWrapper<CameraController> CameraData::getCameraController() const
    {
        return _cameraController;
    }

    hey::ObservableValue<bool>& CameraData::onActiveRotation()
    {
        return _activeRotation;
    }

    const hey::ObservableValue<bool>& CameraData::onActiveRotation() const
    {
        return _activeRotation;
    }

    hey::ObservableValue<bool>& CameraData::onActivePosition()
    {
        return _activePosition;
    }

    const hey::ObservableValue<bool>& CameraData::onActivePosition() const
    {
        return _activePosition;
    }
} // namespace neoneuron
