//
// Created by gaeqs on 16/10/24.
//

#include "OrbitalCameraController.h"

#include <neoneuron/render/NeoneuronRender.h>

#include "CameraData.h"

namespace neoneuron {
    void OrbitalCameraController::sendPosition() const {
        auto rotation = rush::Quatf::euler(rush::Vec3f(_position.rotation, 0.0f));
        auto result = _position.position + rotation * rush::Vec3f(0.0f, 0.0f, _position.radius);

        _interpolator->setPosition(result);
        _interpolator->setRotation(rotation);
    }

    void OrbitalCameraController::dragPosition(const neon::CursorMoveEvent& event) {
        rush::Vec3f delta = {event.delta * rush::Vec2f(-_positionScale, _positionScale), 0.0f};

        delta = getGameObject()->getRoom()->getCamera().getRotation() * delta;

        _position.position += delta;
        _velocity = delta / getApplication()->getCurrentFrameInformation().currentDeltaTime;

        sendPosition();
    }

    void OrbitalCameraController::dragRotation(const neon::CursorMoveEvent& event) {
        constexpr float SCALE = -0.003f;

        rush::Vec2f delta = event.delta(1, 0) * SCALE;

        _position.rotation += delta;
        _position.rotation.x() = std::clamp(
            _position.rotation.x(),
            -std::numbers::pi_v<float> / 2.0f - 0.0001f,
            std::numbers::pi_v<float> / 2.0f + 0.0001f
        );

        _angularVelocity = delta / getApplication()->getCurrentFrameInformation().currentDeltaTime;

        sendPosition();
    }

    bool OrbitalCameraController::updatePosition(float deltaTime) {
        constexpr float D = 10.0f;

        if (_velocity.squaredLength() < 0.01f) {
            _velocity = rush::Vec3f();
            return false;
        }

        auto force = -D * _velocity;

        _velocity += force * deltaTime;
        _position.position += _velocity * deltaTime;
        return true;
    }

    bool OrbitalCameraController::updateRotation(float deltaTime) {
        constexpr float D = 10.0f;

        if (_dragRotation || _angularVelocity.squaredLength() < 0.01f) {
            _angularVelocity = rush::Vec2f(); // Reset when not moving.
            return false;
        }

        auto force = -D * _angularVelocity;

        _angularVelocity += force * deltaTime;
        _position.rotation += _angularVelocity * deltaTime;
        _position.rotation.x() = std::clamp(
            _position.rotation.x(),
            -std::numbers::pi_v<float> / 2.0f - 0.0001f,
            std::numbers::pi_v<float> / 2.0f + 0.0001f
        );

        return true;
    }

    bool OrbitalCameraController::updateRadius(float deltaTime) {
        constexpr float D = 10.0f;

        if (std::abs(_radiusVelocity) <= 0.001f) {
            _radiusVelocity = 0.0f;
            return false;
        }

        auto force = -D * _radiusVelocity;
        _radiusVelocity += force * deltaTime;
        _position.radius += _radiusVelocity * deltaTime;
        if (_position.radius < 0.01f) {
            _position.radius = 0.01f;
            _radiusVelocity = 0.0f;
        }

        return true;
    }

    OrbitalCameraController::
    OrbitalCameraController(NeoneuronRender* render,
                            CameraData* cameraData,
                            std::unique_ptr<CameraInterpolator> interpolator)
        : CameraController(cameraData),
          _render(render),
          _interpolator(std::move(interpolator)),
          _position(rush::Vec3f(), rush::Vec2f(), 10.0f),
          _radiusVelocity(0),
          _radiusScale(_position.radius / 10.0f),
          _positionScale(_position.radius / 1000.0f),
          _dragPosition(false),
          _dragRotation(false) {
        sendPosition();
    }

    OrbitalCameraController::~OrbitalCameraController() {
        for (auto guide: _guides) {
            guide->destroy();
        }
    }

    void OrbitalCameraController::focusOn(const rush::AABB<3, float>& aabb) {
        _position.position = aabb.center;
        _position.radius = aabb.radius.length();
        _radiusScale = _position.radius / 10.0f;
        _positionScale = _position.radius / 30000.0f;
        sendPosition();
    }

    void OrbitalCameraController::forcePosition(rush::Vec3f position) {
        _position.position = position;
        sendPosition();
    }

    void OrbitalCameraController::setInterpolator(std::unique_ptr<CameraInterpolator> interpolator) {
        _interpolator = std::move(interpolator);
        sendPosition();
    }

    void OrbitalCameraController::onMouseButton(const neon::MouseButtonEvent& event) {
        if (event.action == neon::KeyboardAction::PRESS &&
            (!_render->getUI().getViewport()->isHovered() ||
             !event.isModifierActive(neon::KeyboardModifier::CONTROL))) {
            return;
        }
        if (event.button == neon::MouseButton::BUTTON_SECONDARY) {
            _dragPosition = event.action == neon::KeyboardAction::PRESS;
            if (_dragPosition) {
                _dragRotation = false;
            }
        }
        if (event.button == neon::MouseButton::BUTTON_PRIMARY) {
            _dragRotation = event.action == neon::KeyboardAction::PRESS;
            if (_dragRotation) {
                _dragPosition = false;
            }
        }

        if (_cameraData->onActivePosition().getValue() != _dragPosition) {
            _cameraData->onActivePosition().setValue(_dragPosition);
        }

        if (_cameraData->onActiveRotation().getValue() != _dragRotation) {
            _cameraData->onActiveRotation().setValue(_dragRotation);
        }
    }

    void OrbitalCameraController::onCursorMove(const neon::CursorMoveEvent& event) {
        if (_dragPosition) dragPosition(event);
        if (_dragRotation) dragRotation(event);
    }

    void OrbitalCameraController::onScroll(const neon::ScrollEvent& event) {
        if (!_render->getUI().getViewport()->isHovered()) return;

        _radiusVelocity -= event.delta.y() * _radiusScale;
    }

    void OrbitalCameraController::onStart() {
        _guides.push_back(getGameObject()->newComponent<PlaneGuide>(_render));
        _guides.push_back(getGameObject()->newComponent<SphereGuide>(_render, this));
        _guides.push_back(getGameObject()->newComponent<PointGuide>(_render, this));
    }

    void OrbitalCameraController::onUpdate(float deltaTime) {
        _interpolator->onUpdate(deltaTime);
        bool update = updatePosition(deltaTime);
        update |= updateRotation(deltaTime);
        update |= updateRadius(deltaTime);
        if (update) sendPosition();
    }

    float OrbitalCameraController::getRadius() const {
        return _position.radius;
    }

    rush::Vec3f OrbitalCameraController::getCenter() const {
        return _position.position;
    }

    rush::Vec3f OrbitalCameraController::getFinalPosition() const {
        auto rotation = rush::Quatf::euler(rush::Vec3f(_position.rotation, 0.0f));
        return _position.position + rotation * rush::Vec3f(0.0f, 0.0f, _position.radius);
    }

    void OrbitalCameraController::setCenterKeepPosition(rush::Vec3f center) {
        auto position = getFinalPosition();
        _position.position = center;
        _position.radius = (position - center).length();

        auto lookAt = (position - center).normalized();
        float pitch = -std::asin(lookAt.y());
        float yaw = std::atan2(lookAt.x(), lookAt.z());

        _position.rotation = {pitch, yaw};

        sendPosition();
    }
}
