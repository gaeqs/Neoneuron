//
// Created by gaelr on 04/11/2022.
//

#include "GlobalParametersUpdaterComponent.h"

#include <neoneuron/render/NeoneuronRender.h>

#include "camera/OrbitalCameraController.h"

namespace neoneuron {
    GlobalParametersUpdaterComponent::GlobalParametersUpdaterComponent(NeoneuronRender& render) : _render(render) {}

    GlobalParametersUpdaterComponent::~GlobalParametersUpdaterComponent() = default;

    void GlobalParametersUpdaterComponent::onStart() {}

    void GlobalParametersUpdaterComponent::onUpdate(float deltaTime) {
        auto& camera = getRoom()->getCamera();

        auto& buffer = getApplication()->getRender()->getGlobalUniformBuffer();

        buffer.uploadData<Matrices>(
            0,
            Matrices{
                camera.getView(),
                camera.getViewProjection(),
                camera.getFrustum().getInverseProjection(),
                camera.getFrustum().getNear(),
                camera.getFrustum().getFar()
            }
        );


        buffer.uploadData<Time>(
            1,
            Time{_render.getCurrentTime()}
        );

        auto bb = _render.getNeuronScene().getSceneBoundingBox();

        float radius = 0.0f;
        rush::Vec3f center = camera.getPosition();
        auto controller = _render.getCameraData().getCameraController();
        if (auto* orb = dynamic_cast<OrbitalCameraController*>(controller.raw())) {
            radius = orb->getRadius();
            center = orb->getCenter();
        }

        buffer.uploadData<Scene>(
            2,
            Scene{
                bb.center,
                radius,
                bb.radius,
                0,
                center
            }
        );
    }
}
