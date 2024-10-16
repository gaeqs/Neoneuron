//
// Created by gaeqs on 16/10/24.
//

#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#include <neon/Neon.h>

#include <neoneuron/render/component/camera/CameraInterpolator.h>

namespace neoneuron {
    class CameraController : public neon::Component {
    public:
        virtual void focusOn(const rush::AABB<3, float>& aabb) = 0;

        virtual void forcePosition(rush::Vec3f position);

        virtual void setInterpolator(std::unique_ptr<CameraInterpolator> interpolator);
    };
}


#endif //CAMERACONTROLLER_H
