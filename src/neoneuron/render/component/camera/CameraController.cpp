//
// Created by gaeqs on 16/10/24.
//

#include "CameraController.h"

namespace neoneuron {
    hey::ObservableValue<bool>& CameraController::onActiveRotation() {
        return _activeRotation;
    }

    const hey::ObservableValue<bool>& CameraController::onActiveRotation() const {
        return _activeRotation;
    }

    hey::ObservableValue<bool>& CameraController::onActivePosition() {
        return _activePosition;
    }

    const hey::ObservableValue<bool>& CameraController::onActivePosition() const {
        return _activePosition;
    }
}
