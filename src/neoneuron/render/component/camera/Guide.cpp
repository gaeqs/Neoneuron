//
// Created by gaeqs on 23/10/2024.
//

#include "Guide.h"

#include <neoneuron/render/NeoneuronRender.h>

#include "OrbitalCameraController.h"

CMRC_DECLARE(resources);

namespace neoneuron {
    void PlaneGuide::updatePlaneState(bool active) const {
        _planeModel->getInstanceData(0)->uploadData(
            _planeInstance, 0, GuideInstancingData{
                active ? 1.0f : 0.0f,
                _render->getCurrentTime()
            }
        );
    }

    PlaneGuide::PlaneGuide(NeoneuronRender* render)
        : _render(render),
          _positionListener([this](bool active) {
              updatePlaneState(active);
          }) {}

    PlaneGuide::~PlaneGuide() {
        if (_planeModel != nullptr) {
            getRoom()->unmarkUsingModel(_planeModel.get());
        }
    }

    void PlaneGuide::onStart() {
        neon::CMRCFileSystem fs(cmrc::resources::get_filesystem());
        neon::AssetLoaderContext context(getApplication());
        context.fileSystem = &fs;

        _planeModel = neon::loadAssetFromFile<neon::Model>("/model/guide/plane_guide.json", context);
        getRoom()->markUsingModel(_planeModel.get());
        _planeInstance = _planeModel->getInstanceData(0)->createInstance().getResult();

        // Init Hey!
        _render->getCameraData().onActivePosition() += _positionListener;

        updatePlaneState(false);
    }


    void SphereGuide::updateSphereState() const {
        _sphereModel->getInstanceData(0)->uploadData(
            _sphereInstance, 0, GuideInstancingData{
                _state ? 1.0f : 0.0f,
                _lastUpdate,
                _orbitalController->getCenter(),
                _orbitalController->getRadius()
            }
        );
    }

    SphereGuide::SphereGuide(NeoneuronRender* render, neon::IdentifiableWrapper<OrbitalCameraController> controller)
        : _render(render),
          _orbitalController(controller),
          _rotationListener([this](bool active) {
              _state = active;
              _lastUpdate = _render->getCurrentTime();
              updateSphereState();
          }),
          _state(false) {}

    SphereGuide::~SphereGuide() {
        if (_sphereModel != nullptr) {
            getRoom()->unmarkUsingModel(_sphereModel.get());
        }
    }

    void SphereGuide::onStart() {
        neon::CMRCFileSystem fs(cmrc::resources::get_filesystem());
        neon::AssetLoaderContext context(getApplication());
        context.fileSystem = &fs;

        _sphereModel = neon::loadAssetFromFile<neon::Model>("/model/guide/sphere_guide.json", context);

        getRoom()->markUsingModel(_sphereModel.get());

        _sphereInstance = _sphereModel->getInstanceData(0)->createInstance().getResult();

        // Init Hey!
        _render->getCameraData().onActiveRotation() += _rotationListener;

        updateSphereState();
    }

    void SphereGuide::onPreDraw() {
        updateSphereState();
    }

    void PointGuide::updatePointState() {
        bool newState = _positionState | _rotationState;
        if (newState != _state) {
            _state = newState;
            _lastUpdate = _render->getCurrentTime();
        }

        _sphereModel->getInstanceData(0)->uploadData(
            _sphereInstance, 0, GuideInstancingData{
                _state ? 1.0f : 0.0f,
                _lastUpdate,
                _orbitalController->getCenter()
            }
        );
    }

    PointGuide::PointGuide(NeoneuronRender* render, neon::IdentifiableWrapper<OrbitalCameraController> controller)
        : _render(render),
          _orbitalController(controller),
          _positionListener([this](bool active) {
              _positionState = active;
              updatePointState();
          }),
          _rotationListener([this](bool active) {
              _rotationState = active;
              updatePointState();
          }),
          _positionState(false),
          _rotationState(false),
          _state(false) {}

    PointGuide::~PointGuide() {
        if (_sphereModel != nullptr) {
            getRoom()->unmarkUsingModel(_sphereModel.get());
        }
    }

    void PointGuide::onStart() {
        neon::CMRCFileSystem fs(cmrc::resources::get_filesystem());
        neon::AssetLoaderContext context(getApplication());
        context.fileSystem = &fs;

        _sphereModel = neon::loadAssetFromFile<neon::Model>("/model/guide/point_guide.json", context);

        getRoom()->markUsingModel(_sphereModel.get());

        _sphereInstance = _sphereModel->getInstanceData(0)->createInstance().getResult();

        // Init Hey!
        _render->getCameraData().onActiveRotation() += _rotationListener;
        _render->getCameraData().onActivePosition() += _positionListener;

        updatePointState();
    }

    void PointGuide::onPreDraw() {
        updatePointState();
    }
}
