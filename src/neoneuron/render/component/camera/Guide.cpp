// Copyright (c) 2025. VG-Lab/URJC.
//
// Authors: Gael Rial Costas <gael.rial.costas@urjc.es>
//
// This file is part of Neoneuron <gitlab.gmrv.es/g.rial/neoneuron>
//
// This library is free software; you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License version 3.0 as published
// by the Free Software Foundation.
//
// This library is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this library; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

#include "Guide.h"

#include <neoneuron/render/NeoneuronRender.h>

#include "OrbitalCameraController.h"

CMRC_DECLARE(resources);

namespace neoneuron
{
    void PlaneGuide::updatePlaneState(bool active) const
    {
        _planeModel->getInstanceData(0)->uploadData(
            _planeInstance, 0, GuideInstancingData{active ? 1.0f : 0.0f, _viewport->getRender()->getCurrentTime()});
    }

    PlaneGuide::PlaneGuide(Viewport* viewport) :
        _viewport(viewport),
        _planeInstance(),
        _positionListener([this](bool active) { updatePlaneState(active); })
    {
    }

    PlaneGuide::~PlaneGuide()
    {
        if (_planeModel != nullptr) {
            getRoom()->unmarkUsingModel(_planeModel.get());
        }
    }

    void PlaneGuide::onStart()
    {
        neon::AssetCollection localCollection;
        localCollection.store(_viewport->getInputFrameBuffer(), neon::AssetStorageMode::WEAK);
        localCollection.store(_viewport->getUniformDescriptor(), neon::AssetStorageMode::WEAK);
        localCollection.store(_viewport->getUniformBuffer(), neon::AssetStorageMode::WEAK);

        neon::CMRCFileSystem fs(cmrc::resources::get_filesystem());
        neon::AssetLoaderContext context(getApplication());
        context.fileSystem = &fs;
        context.localCollection = &localCollection;

        _planeModel = neon::loadAssetFromFile<neon::Model>("/model/guide/plane_guide.json", context);
        getRoom()->markUsingModel(_planeModel.get());
        _planeInstance = _planeModel->getInstanceData(0)->createInstance().getResult();

        // Init Hey!
        _viewport->getCameraData().onActivePosition() += _positionListener;

        updatePlaneState(false);
    }

    void SphereGuide::updateSphereState() const
    {
        _sphereModel->getInstanceData(0)->uploadData(_sphereInstance, 0,
                                                     GuideInstancingData{_state ? 1.0f : 0.0f, _lastUpdate,
                                                                         _orbitalController->getCenter(),
                                                                         _orbitalController->getRadius()});
    }

    SphereGuide::SphereGuide(Viewport* viewport, neon::IdentifiableWrapper<OrbitalCameraController> controller) :
        _viewport(viewport),
        _orbitalController(std::move(controller)),
        _sphereInstance(),
        _rotationListener([this](bool active) {
            _state = active;
            _lastUpdate = _viewport->getRender()->getCurrentTime();
            updateSphereState();
        }),
        _state(false),
        _lastUpdate(0)
    {
    }

    SphereGuide::~SphereGuide()
    {
        if (_sphereModel != nullptr) {
            getRoom()->unmarkUsingModel(_sphereModel.get());
        }
    }

    void SphereGuide::onStart()
    {
        neon::AssetCollection localCollection;
        localCollection.store(_viewport->getInputFrameBuffer(), neon::AssetStorageMode::WEAK);
        localCollection.store(_viewport->getUniformDescriptor(), neon::AssetStorageMode::WEAK);
        localCollection.store(_viewport->getUniformBuffer(), neon::AssetStorageMode::WEAK);

        neon::CMRCFileSystem fs(cmrc::resources::get_filesystem());
        neon::AssetLoaderContext context(getApplication());
        context.fileSystem = &fs;
        context.localCollection = &localCollection;

        _sphereModel = neon::loadAssetFromFile<neon::Model>("/model/guide/sphere_guide.json", context);

        getRoom()->markUsingModel(_sphereModel.get());

        _sphereInstance = _sphereModel->getInstanceData(0)->createInstance().getResult();

        // Init Hey!
        _viewport->getCameraData().onActiveRotation() += _rotationListener;

        updateSphereState();
    }

    void SphereGuide::onPreDraw()
    {
        updateSphereState();
    }

    void PointGuide::updatePointState()
    {
        bool newState = _positionState | _rotationState;
        if (newState != _state) {
            _state = newState;
            _lastUpdate = _viewport->getRender()->getCurrentTime();
        }

        _sphereModel->getInstanceData(0)->uploadData(
            _sphereInstance, 0,
            GuideInstancingData{_state ? 1.0f : 0.0f, _lastUpdate, _orbitalController->getCenter()});
    }

    PointGuide::PointGuide(Viewport* viewport, neon::IdentifiableWrapper<OrbitalCameraController> controller) :
        _viewport(viewport),
        _orbitalController(std::move(controller)),
        _sphereInstance(),
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
        _state(false),
        _lastUpdate(0)
    {
    }

    PointGuide::~PointGuide()
    {
        if (_sphereModel != nullptr) {
            getRoom()->unmarkUsingModel(_sphereModel.get());
        }
    }

    void PointGuide::onStart()
    {
        neon::AssetCollection localCollection;
        localCollection.store(_viewport->getInputFrameBuffer(), neon::AssetStorageMode::WEAK);
        localCollection.store(_viewport->getUniformDescriptor(), neon::AssetStorageMode::WEAK);
        localCollection.store(_viewport->getUniformBuffer(), neon::AssetStorageMode::WEAK);

        neon::CMRCFileSystem fs(cmrc::resources::get_filesystem());
        neon::AssetLoaderContext context(getApplication());
        context.fileSystem = &fs;
        context.localCollection = &localCollection;

        _sphereModel = neon::loadAssetFromFile<neon::Model>("/model/guide/point_guide.json", context);

        getRoom()->markUsingModel(_sphereModel.get());

        _sphereInstance = _sphereModel->getInstanceData(0)->createInstance().getResult();

        // Init Hey!
        _viewport->getCameraData().onActiveRotation() += _rotationListener;
        _viewport->getCameraData().onActivePosition() += _positionListener;

        updatePointState();
    }

    void PointGuide::onPreDraw()
    {
        updatePointState();
    }
} // namespace neoneuron
