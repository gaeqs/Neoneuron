//
// Created by gaeqs on 23/10/2024.
//

#include "Guide.h"

#include <neoneuron/render/NeoneuronRender.h>

CMRC_DECLARE(resources);

namespace neoneuron {
    void Guide::updatePlaneState(bool active) const {
        _planeModel->getInstanceData(0)->uploadData(
            _planeInstance, 0, GuideInstancingData{
                active ? 1.0f : 0.0f,
                _render->getCurrentTime()
            }
        );
    }

    void Guide::updateSphereState(bool active) const {
        _sphereModel->getInstanceData(0)->uploadData(
            _sphereInstance, 0, GuideInstancingData{
                active ? 1.0f : 0.0f,
                _render->getCurrentTime()
            }
        );
    }

    Guide::Guide(NeoneuronRender* render)
        : _render(render),
          _positionListener([this](bool active) {
              updatePlaneState(active);
          }),
          _rotationListener([this](bool active) {
              updateSphereState(active);
          }) {}

    Guide::~Guide() {
        if (_planeModel != nullptr) {
            getRoom()->unmarkUsingModel(_planeModel.get());
        }
        if(_sphereModel != nullptr) {
            getRoom()->unmarkUsingModel(_sphereModel.get());
        }
    }

    void Guide::onStart() {
        neon::CMRCFileSystem fs(cmrc::resources::get_filesystem());
        neon::AssetLoaderContext context(getApplication());
        context.fileSystem = &fs;

        _planeModel = neon::loadAssetFromFile<neon::Model>("/model/guide/plane_guide.json", context);
        _sphereModel = neon::loadAssetFromFile<neon::Model>("/model/guide/sphere_guide.json", context);

        getRoom()->markUsingModel(_planeModel.get());
        getRoom()->markUsingModel(_sphereModel.get());

        _planeInstance = _planeModel->getInstanceData(0)->createInstance().getResult();
        _sphereInstance = _sphereModel->getInstanceData(0)->createInstance().getResult();

        // Init Hey!
        _render->getCameraData().onActivePosition() += _positionListener;
        _render->getCameraData().onActiveRotation() += _rotationListener;

        updatePlaneState(false);
        updateSphereState(false);
    }
}
