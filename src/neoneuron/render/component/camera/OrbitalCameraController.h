//
// Created by gaeqs on 16/10/24.
//

#ifndef ORBITALCAMERACONTROLLER_H
#define ORBITALCAMERACONTROLLER_H

#include <neoneuron/render/component/camera/CameraController.h>

namespace neoneuron
{
    class NeoneuronRender;

    class OrbitalCameraController : public CameraController
    {
        struct Position
        {
            rush::Vec3f position;
            rush::Vec2f rotation;
            float radius;
        };

        NeoneuronRender* _render;
        std::unique_ptr<CameraInterpolator> _interpolator;

        Position _position;

        rush::Vec3f _velocity;
        rush::Vec2f _angularVelocity;
        float _radiusVelocity;

        float _radiusScale;
        float _positionScale;

        bool _dragPosition;
        bool _dragRotation;

        std::vector<neon::IdentifiableWrapper<Component>> _guides;

        void sendPosition() const;

        void dragPosition(const neon::CursorMoveEvent& event);

        void dragRotation(const neon::CursorMoveEvent& event);

        bool updatePosition(float deltaTime);

        bool updateRotation(float deltaTime);

        bool updateRadius(float deltaTime);

      public:
        OrbitalCameraController(NeoneuronRender* render, CameraData* cameraData,
                                std::unique_ptr<CameraInterpolator> interpolator);

        ~OrbitalCameraController();

        void focusOn(const rush::AABB<3, float>& aabb) override;

        void forcePosition(rush::Vec3f position) override;

        void setInterpolator(std::unique_ptr<CameraInterpolator> interpolator) override;

        void onMouseButton(const neon::MouseButtonEvent& event) override;

        void onCursorMove(const neon::CursorMoveEvent& event) override;

        void onScroll(const neon::ScrollEvent& event) override;

        void onStart() override;

        void onUpdate(float deltaTime) override;

        float getRadius() const;

        [[nodiscard]] rush::Vec3f getCenter() const;

        [[nodiscard]] rush::Vec3f getFinalPosition() const;

        void setCenterKeepPosition(rush::Vec3f center);
    };
} // namespace neoneuron

#endif //ORBITALCAMERACONTROLLER_H
