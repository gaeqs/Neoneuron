//
// Created by gaeqs on 16/10/24.
//

#ifndef ORBITALCAMERACONTROLLER_H
#define ORBITALCAMERACONTROLLER_H


#include <neoneuron/render/component/camera/CameraController.h>


class OrbitalCameraController : public neoneuron::CameraController {
    rush::Vec3f position;


public:
    void focusOn(const rush::AABB<3, float>& aabb) override;

    void forcePosition(rush::Vec3f position) override;
};


#endif //ORBITALCAMERACONTROLLER_H
