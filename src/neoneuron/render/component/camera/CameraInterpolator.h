//
// Created by gaeqs on 16/10/24.
//

#ifndef CAMERAINTERPOLATOR_H
#define CAMERAINTERPOLATOR_H
#include <rush/quaternion/quat.h>
#include <rush/vector/vec.h>

namespace neoneuron
{
    class CameraInterpolator
    {
      public:
        virtual ~CameraInterpolator() = default;

        virtual void setPosition(rush::Vec3f position) = 0;

        virtual void teleport(rush::Vec3f position) = 0;

        virtual void setRotation(rush::Quatf rotation) = 0;

        virtual void teleportRotation(rush::Quatf rotation) = 0;

        virtual void onUpdate(float deltaTime) = 0;
    };
} // namespace neoneuron

#endif //CAMERAINTERPOLATOR_H
