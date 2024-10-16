//
// Created by gaeqs on 16/10/24.
//

#ifndef INSTANTCAMERAINTERPOLATOR_H
#define INSTANTCAMERAINTERPOLATOR_H

#include <neon/Neon.h>

#include <neoneuron/render/component/camera/CameraInterpolator.h>


namespace neoneuron {
    class InstantCameraInterpolator : public CameraInterpolator {
        neon::Camera* _camera;

    public:
        explicit InstantCameraInterpolator(neon::Camera* camera);

        void setPosition(rush::Vec3f position) override;

        void teleport(rush::Vec3f position) override;
    };
}


#endif //INSTANTCAMERAINTERPOLATOR_H
