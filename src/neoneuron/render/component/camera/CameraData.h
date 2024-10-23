//
// Created by gaeqs on 22/10/2024.
//

#ifndef CAMERADATA_H
#define CAMERADATA_H

#include <neon/Neon.h>
#include <neoneuron/render/component/camera/CameraController.h>

namespace neoneuron {
    class CameraData {
        neon::IdentifiableWrapper<neon::GameObject> _cameraGameObject;
        neon::IdentifiableWrapper<CameraController> _cameraController;

    public:
        CameraData() = default;

        explicit CameraData(neon::Room* room);

        [[nodiscard]] neon::IdentifiableWrapper<CameraController> cameraController() const;
    };
}


#endif //CAMERADATA_H
