//
// Created by gaeqs on 16/10/24.
//

#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#include <neon/Neon.h>

#include <neoneuron/render/component/camera/CameraInterpolator.h>

namespace neoneuron
{
    class CameraData;

    class CameraController : public neon::Component
    {
      protected:
        CameraData* _cameraData;

      public:
        explicit CameraController(CameraData* cameraData);

        virtual void focusOn(const rush::AABB<3, float>& aabb) = 0;

        virtual void forcePosition(rush::Vec3f position) = 0;

        virtual void setInterpolator(std::unique_ptr<CameraInterpolator> interpolator) = 0;
    };
} // namespace neoneuron

#endif //CAMERACONTROLLER_H
