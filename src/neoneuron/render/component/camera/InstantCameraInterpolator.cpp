//
// Created by gaeqs on 16/10/24.
//

#include "InstantCameraInterpolator.h"

neoneuron::InstantCameraInterpolator::InstantCameraInterpolator(neon::Camera* camera)
    : _camera(camera) {}

void neoneuron::InstantCameraInterpolator::setPosition(rush::Vec3f position) {
    _camera->setPosition(position);
}

void neoneuron::InstantCameraInterpolator::teleport(rush::Vec3f position) {
    _camera->setPosition(position);
}
