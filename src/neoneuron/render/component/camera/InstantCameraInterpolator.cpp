//
// Created by gaeqs on 16/10/24.
//

#include "InstantCameraInterpolator.h"

neoneuron::InstantCameraInterpolator::InstantCameraInterpolator(neon::Camera* camera) :
    _camera(camera)
{
}

void neoneuron::InstantCameraInterpolator::setPosition(rush::Vec3f position)
{
    _camera->setPosition(position);
}

void neoneuron::InstantCameraInterpolator::teleport(rush::Vec3f position)
{
    _camera->setPosition(position);
}

void neoneuron::InstantCameraInterpolator::setRotation(rush::Quatf rotation)
{
    _camera->setRotation(rotation);
}

void neoneuron::InstantCameraInterpolator::teleportRotation(rush::Quatf rotation)
{
    _camera->setRotation(rotation);
}

void neoneuron::InstantCameraInterpolator::onUpdate(float deltaTime)
{
}
