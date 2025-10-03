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

#include "ViewportNode.h"

#include "neoneuron/render/component/camera/CameraController.h"

CMRC_DECLARE(resources);

namespace
{
    std::shared_ptr<neon::SampledTexture> loadSkybox(neon::Room* room)
    {
        neon::CMRCFileSystem fileSystem(cmrc::resources::get_filesystem());
        neon::AssetLoaderContext context(room->getApplication(), nullptr, &fileSystem);
        return loadAssetFromFile<neon::SampledTexture>("texture/white_skybox/skybox.json", context);
    }
} // namespace

namespace neoneuron
{

    ViewportNode::ViewportNode(NeoneuronApplication* application) :
        Node("Viewport"),
        _application(application),
        _viewport(application->getRender().addViewport())
    {
        defineInput<neon::IdentifiableWrapper<CameraController>>("Camera", false);
        defineOutput<Viewport*>("Viewport", _viewport);
        _viewport->setSkybox(loadSkybox(application->getRender().getRoom().get()));
    }

    ViewportNode::~ViewportNode()
    {
        sendOutput("Viewport", std::any());
        _application->getRender().removeViewport(_viewport);
    }

    void ViewportNode::onInputChange(const std::string& name, const std::any& value)
    {
        if (name == "Camera") {
            auto* cameraPtr = std::any_cast<neon::IdentifiableWrapper<CameraController>>(&value);
            if (cameraPtr != nullptr) {
                _viewport->setExternalCamera(*cameraPtr);
            } else {
                _viewport->setExternalCamera(nullptr);
            }
        }
    }

    NodeFactory ViewportNode::createFactory()
    {
        return NodeFactory("Viewport",
                           [](ImBlueprint::Editor& editor, auto* app) { return editor.addNode<ViewportNode>(app); });
    }
} // namespace neoneuron