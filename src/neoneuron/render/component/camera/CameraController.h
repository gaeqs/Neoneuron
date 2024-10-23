//
// Created by gaeqs on 16/10/24.
//

#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#include <hey/Hey.h>
#include <neon/Neon.h>

#include <neoneuron/render/component/camera/CameraInterpolator.h>

namespace neoneuron {
    class CameraController : public neon::Component {
    protected:
        hey::ObservableValue<bool> _activeRotation = false;
        hey::ObservableValue<bool> _activePosition = false;

    public:
        virtual void focusOn(const rush::AABB<3, float>& aabb) = 0;

        virtual void forcePosition(rush::Vec3f position) = 0;

        virtual void setInterpolator(std::unique_ptr<CameraInterpolator> interpolator) = 0;

        hey::ObservableValue<bool>& onActiveRotation();

        const hey::ObservableValue<bool>& onActiveRotation() const;

        hey::ObservableValue<bool>& onActivePosition();

        const hey::ObservableValue<bool>& onActivePosition() const;
    };
}


#endif //CAMERACONTROLLER_H
