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

#ifndef NEONEURONRENDER_H
#define NEONEURONRENDER_H

#include <neon/Neon.h>

#include <neoneuron/ui/NeoneuronUI.h>
#include <neoneuron/render/AbstractNeuronRepresentation.h>
#include <neoneuron/render/NeoneuronRenderData.h>

namespace neoneuron
{
    class NeoneuronApplication;

    class NeoneuronRender
    {
        struct Components
        {
            NeoneuronUI ui;

            Components(NeoneuronRender* render);
        };

        NeoneuronApplication* _neoneuronApplication;
        neon::Application _application;
        neon::CMRCFileSystem _fileSystem;
        std::shared_ptr<neon::Room> _room;
        std::chrono::high_resolution_clock::time_point _startTime;
        std::unique_ptr<Components> _components;
        NeoneuronRenderData _renderData;

        std::vector<std::shared_ptr<AbstractNeuronRepresentation>> _representations;

        neon::IdentifiableWrapper<neon::GameObject> _viewportGO;
        std::vector<Viewport*> _viewports;

        std::shared_ptr<neon::Render> initRender();

        void initGameObjects();

      public:
        NeoneuronRender(const NeoneuronRender& other) = delete;

        explicit NeoneuronRender(NeoneuronApplication* neoneuron,
                                 const neon::vulkan::VKApplicationCreateInfo& createInfo);

        ~NeoneuronRender();

        [[nodiscard]] NeoneuronApplication* getNeoneuronApplication();

        [[nodiscard]] const NeoneuronApplication* getNeoneuronApplication() const;

        [[nodiscard]] neon::Application& getApplication();

        [[nodiscard]] const neon::Application& getApplication() const;

        const neon::CMRCFileSystem getFileSystem() const;

        [[nodiscard]] const std::shared_ptr<neon::Room>& getRoom() const;

        [[nodiscard]] NeoneuronUI& getUI();

        [[nodiscard]] const NeoneuronUI& getUI() const;

        [[nodiscard]] NeoneuronRenderData& getRenderData();

        [[nodiscard]] const NeoneuronRenderData& getRenderData() const;

        [[nodiscard]] std::vector<AbstractNeuronRepresentation*> getRepresentations();

        [[nodiscard]] std::vector<const AbstractNeuronRepresentation*> getRepresentations() const;

        rush::AABB<3, float> getCombinedAABB() const;

        float getCurrentTime() const;

        bool renderLoop();

        void focusScene() const;

        void refreshNeuronProperty(GID neuronId, const std::string& propertyName);

        template<typename Representation>
        std::weak_ptr<Representation> addRepresentation()
        {
            auto ptr = std::make_shared<Representation>(this);
            _representations.push_back(ptr);
            return ptr;
        }

        void removeRepresentation(AbstractNeuronRepresentation* representation);

        Viewport* addViewport();

        void removeViewport(Viewport* viewport);

        auto getViewports() const
        {
            return _viewports | std::views::transform([](const Viewport* viewport) { return viewport; });
        }
    };
} // namespace neoneuron

#endif // NEONEURONRENDER_H
