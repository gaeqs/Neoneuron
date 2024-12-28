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

    NeuronTransform::NeuronTransform(const rush::Mat4f& model) {
        _position = model[3](0, 1, 2);
        _scale.x() = model[0](0, 1, 2).toVec().length();
        _scale.y() = model[1](0, 1, 2).toVec().length();
        _scale.z() = model[2](0, 1, 2).toVec().length();

        auto rot = rush::Mat3f(model);
        rot[0] /= _scale.x();
        rot[1] /= _scale.y();
        rot[2] /= _scale.z();

        std::cout << rot << std::endl;

        _rotation = rush::Quatf::fromRotationMatrix(rot);
    }

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
