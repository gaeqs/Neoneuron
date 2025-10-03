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

#include "NeoneuronRender.h"

#include <vector>

#include <neon/util/DeferredUtils.h>

#include <neoneuron/render/component/camera/CameraController.h>
#include <neoneuron/render/component/GlobalParametersUpdaterComponent.h>
#include <neoneuron/application/NeoneuronApplication.h>
#include <neoneuron/render/complex/ComplexNeuronRepresentation.h>

CMRC_DECLARE(resources);

namespace neoneuron
{
    NeoneuronRender::Components::Components(NeoneuronRender* render) :
        ui(render)
    {
    }

    std::shared_ptr<neon::Render> NeoneuronRender::initRender()
    {
        neon::AssetLoaderContext context(&_application, nullptr, &_fileSystem);
        auto render = neon::loadAssetFromFile<neon::Render>("render.json", context);
        if (render == nullptr) {
            neon::error() << "Render is null!";
        }
        return render;
    }

    void NeoneuronRender::initGameObjects()
    {
        auto parameterUpdaterGO = _room->newGameObject();
        parameterUpdaterGO->setName("Parameter updater");
        parameterUpdaterGO->newComponent<GlobalParametersUpdaterComponent>(*this);
    }

    NeoneuronRender::NeoneuronRender(NeoneuronApplication* neoneuron,
                                     const neon::vulkan::VKApplicationCreateInfo& createInfo) :
        _neoneuronApplication(neoneuron),
        _application(std::make_unique<neon::vulkan::VKApplication>(createInfo)),
        _fileSystem(cmrc::resources::get_filesystem())
    {
        _application.init();
        neoneuron->getFiles().loadImGuiIniFile();
        _application.setRender(initRender());

        _room = std::make_shared<neon::Room>(&_application);
        _application.setRoom(_room);
        _startTime = std::chrono::high_resolution_clock::now();

        _components = std::make_unique<Components>(this);

        _viewportGO = _room->newGameObject();

        initGameObjects();
    }

    NeoneuronRender::~NeoneuronRender()
    {
    }

    NeoneuronApplication* NeoneuronRender::getNeoneuronApplication()
    {
        return _neoneuronApplication;
    }

    const NeoneuronApplication* NeoneuronRender::getNeoneuronApplication() const
    {
        return _neoneuronApplication;
    }

    neon::Application& NeoneuronRender::getApplication()
    {
        return _application;
    }

    const neon::Application& NeoneuronRender::getApplication() const
    {
        return _application;
    }

    const neon::CMRCFileSystem NeoneuronRender::getFileSystem() const
    {
        return _fileSystem;
    }

    const std::shared_ptr<neon::Room>& NeoneuronRender::getRoom() const
    {
        return _room;
    }

    NeoneuronUI& NeoneuronRender::getUI()
    {
        return _components->ui;
    }

    const NeoneuronUI& NeoneuronRender::getUI() const
    {
        return _components->ui;
    }

    NeoneuronRenderData& NeoneuronRender::getRenderData()
    {
        return _renderData;
    }

    const NeoneuronRenderData& NeoneuronRender::getRenderData() const
    {
        return _renderData;
    }

    std::vector<AbstractNeuronRepresentation*> NeoneuronRender::getRepresentations()
    {
        std::vector<AbstractNeuronRepresentation*> result;
        for (auto& rep : _representations) {
            result.push_back(rep.get());
        }

        return result;
    }

    std::vector<const AbstractNeuronRepresentation*> NeoneuronRender::getRepresentations() const
    {
        std::vector<const AbstractNeuronRepresentation*> result;
        for (auto& rep : _representations) {
            result.push_back(rep.get());
        }

        return result;
    }

    rush::AABB<3, float> NeoneuronRender::getCombinedAABB() const
    {
        if (_representations.empty()) {
            return {};
        }
        rush::AABB<3, float> aabb = _representations[0]->getSceneBoundingBox();

        for (size_t i = 1; i < _representations.size(); i++) {
            auto repAABB = _representations[1]->getSceneBoundingBox();
            auto min = rush::min(aabb.center - aabb.radius, repAABB.center - repAABB.radius);
            auto max = rush::max(aabb.center + aabb.radius, repAABB.center + repAABB.radius);
            aabb = rush::AABB<3, float>::fromEdges(min, max);
        }

        return aabb;
    }

    float NeoneuronRender::getCurrentTime() const
    {
        auto now = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::duration<float>>(now - _startTime).count();
    }

    bool NeoneuronRender::renderLoop()
    {
        auto result = _application.startGameLoop();
        if (!result.isOk()) {
            neon::error() << result.getError();
        }
        return result.isOk();
    }

    void NeoneuronRender::focusScene() const
    {
        auto aabb = getCombinedAABB();
        for (auto viewport : _viewports) {
            viewport->getCameraController()->focusOn(aabb);
        }
    }

    void NeoneuronRender::refreshNeuronProperty(GID neuronId, const std::string& propertyName)
    {
        for (auto& rep : _representations) {
            rep->refreshNeuronProperty(neuronId, propertyName);
        }
    }

    void NeoneuronRender::removeRepresentation(AbstractNeuronRepresentation* representation)
    {
        if (representation == nullptr) {
            return;
        }

        auto it = std::ranges::find_if(_representations, [&](const std::shared_ptr<AbstractNeuronRepresentation>& it) {
            return it.get() == representation;
        });
        if (it == _representations.end()) {
            return;
        }

        _representations.erase(it);
    }

    Viewport* NeoneuronRender::addViewport()
    {
        auto ptr = _viewportGO->newComponent<Viewport>(this, 10);
        _viewports.push_back(ptr.raw());
        return ptr.raw();
    }

    void NeoneuronRender::removeViewport(Viewport* viewport)
    {
        if (viewport == nullptr) {
            return;
        }

        auto it = std::ranges::find_if(_viewports, [&](const Viewport* it) { return it == viewport; });
        if (it == _viewports.end()) {
            return;
        }

        _viewports.erase(it);
        viewport->destroy();
    }
} // namespace neoneuron
