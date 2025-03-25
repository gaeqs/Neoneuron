//
// Created by gaeqs on 22/10/2024.
//

#ifndef CAMERADATA_H
#define CAMERADATA_H

#include <neon/Neon.h>
#include <hey/Hey.h>

#include <neoneuron/render/component/camera/CameraController.h>
#include <neoneuron/render/component/camera/Guide.h>

namespace neoneuron
{
    class NeoneuronRender;

    class CameraData
    {
        neon::IdentifiableWrapper<neon::GameObject> _cameraGameObject;
        neon::IdentifiableWrapper<CameraController> _cameraController;

        hey::ObservableValue<bool> _activeRotation = false;
        hey::ObservableValue<bool> _activePosition = false;

      public:
        CameraData(const CameraData& other) = delete;

        CameraData() = default;

        ~CameraData();

        explicit CameraData(NeoneuronRender* room);

        [[nodiscard]] neon::IdentifiableWrapper<CameraController> getCameraController() const;

        hey::ObservableValue<bool>& onActiveRotation();

        const hey::ObservableValue<bool>& onActiveRotation() const;

        hey::ObservableValue<bool>& onActivePosition();

        const hey::ObservableValue<bool>& onActivePosition() const;
    };
} // namespace neoneuron

#endif //CAMERADATA_H
