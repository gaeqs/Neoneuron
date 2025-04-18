//
// Created by gaeqs on 18/04/2025.
//

#ifndef VIEWPORT_H
#define VIEWPORT_H

#include "NeoneuronRender.h"

#include <memory>

#include <neon/Neon.h>

namespace neoneuron
{
    class Viewport
    {
        NeoneuronRender* _render;
        CameraData _cameraData;

        std::shared_ptr<neon::SimpleFrameBuffer> _inputFrameBuffer;
        std::shared_ptr<neon::SimpleFrameBuffer> _outputFrameBuffer;

        std::shared_ptr<neon::RenderPassStrategy> _inputStrategy;
        std::shared_ptr<neon::RenderPassStrategy> _outputStrategy;

        std::shared_ptr<neon::Model> _selectionResolver;

        neon::IdentifiableWrapper<neon::ViewportComponent> _viewportComponent;


        void loadSelectionResolver();

        void loadViewportComponent();

      public:
        Viewport(NeoneuronRender* render, int priority);

        ~Viewport();

        std::shared_ptr<neon::SimpleFrameBuffer> getInputFrameBuffer() const;

        neon::IdentifiableWrapper<neon::ViewportComponent> getViewportComponent() const;

        void setSkybox(const std::shared_ptr<neon::Texture>& skybox) const;
    };
} // namespace neoneuron

#endif // VIEWPORT_H
