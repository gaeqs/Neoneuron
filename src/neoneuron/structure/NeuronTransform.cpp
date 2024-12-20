//
// Created by gaeqs on 18/12/2024.
//

#include "NeuronTransform.h"

namespace neoneuron {
    void NeuronTransform::recalculateIfRequired() const {
        if (!_dirty) return;
        _model = rush::Mat4f::model(_scale, _rotation, _position);
        _normal = rush::Mat4f::normal(_scale, _rotation);

        _dirty = false;
    }

    NeuronTransform::NeuronTransform() :
        _model(1),
        _normal(1),
        _position(0),
        _scale(1),
        _dirty(false) {}

    const rush::Mat4f& NeuronTransform::getModel() const {
        recalculateIfRequired();
        return _model;
    }

    const rush::Mat4f& NeuronTransform::getNormal() const {
        recalculateIfRequired();
        return _normal;
    }

    const rush::Vec3f& NeuronTransform::getPosition() const {
        return _position;
    }

    void NeuronTransform::setPosition(const rush::Vec3f& position) {
        if (_position == position) return;
        _position = position;
        _dirty = true;
    }

    const rush::Quatf& NeuronTransform::getRotation() const {
        return _rotation;
    }

    void NeuronTransform::setRotation(const rush::Quatf& rotation) {
        if (_rotation == rotation) return;
        _rotation = rotation;
        _dirty = true;
    }

    const rush::Vec3f& NeuronTransform::getScale() const {
        return _scale;
    }

    void NeuronTransform::setScale(const rush::Vec3f& scale) {
        if (_scale == scale) return;
        _scale = scale;
        _dirty = true;
    }
}
