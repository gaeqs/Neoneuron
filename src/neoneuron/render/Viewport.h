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

#ifndef NEONEURON_VIEWPORT_H
#define NEONEURON_VIEWPORT_H

#include "component/camera/CameraData.h"
#include "neoneuron/ui/NeoneuronWindow.h"
#include "neoneuron/ui/components/NeuronTexturePicker.h"

#include <memory>
#include <neon/Neon.h>

namespace neoneuron
{

    class CameraController;

    class NeoneuronRender;

    class Viewport : public NeoneuronWindow
    {
        NeoneuronRender* _render;

        std::unique_ptr<neon::Camera> _internalCamera;
        neon::IdentifiableWrapper<CameraController> _internalCameraController;
        neon::IdentifiableWrapper<CameraController> _externalCameraController;
        CameraData _cameraData;

        std::shared_ptr<neon::ShaderUniformDescriptor> _uniformDescriptor;
        std::shared_ptr<neon::ShaderUniformBuffer> _uniformBuffer;

        std::shared_ptr<neon::SimpleFrameBuffer> _inputFrameBuffer;
        std::shared_ptr<neon::SimpleFrameBuffer> _outputFrameBuffer;

        std::shared_ptr<neon::RenderPassStrategy> _inputStrategy;
        std::shared_ptr<neon::RenderPassStrategy> _outputStrategy;

        std::shared_ptr<neon::SampledTexture> _outputColorTexture;

        std::shared_ptr<neon::Model> _selectionResolver;

        neon::IdentifiableWrapper<NeuronTexturePicker> _texturePicker;

        ImVec2 _windowSize;
        ImVec2 _windowOrigin;
        bool _hovered;

        static std::string getUniqueName();

        void loadUniforms();

        void loadSelectionResolver();

        void assignRecreationConditions(neon::SimpleFrameBuffer* frameBuffer) const;

        void renderSidebar();

      public:
        Viewport(const Viewport& other) = delete;

        Viewport(NeoneuronRender* render, int priority, const std::string& name = "");

        ~Viewport() override;

        [[nodiscard]] NeoneuronRender* getRender() const;

        [[nodiscard]] neon::Camera& getCamera();

        [[nodiscard]] const neon::Camera& getCamera() const;

        [[nodiscard]] neon::IdentifiableWrapper<CameraController> getCameraController() const;

        void setExternalCamera(neon::IdentifiableWrapper<CameraController> camera);

        [[nodiscard]] CameraData& getCameraData();

        [[nodiscard]] const CameraData& getCameraData() const;

        [[nodiscard]] std::shared_ptr<neon::SimpleFrameBuffer> getInputFrameBuffer() const;

        [[nodiscard]] std::shared_ptr<neon::ShaderUniformDescriptor> getUniformDescriptor() const;

        [[nodiscard]] std::shared_ptr<neon::ShaderUniformBuffer> getUniformBuffer() const;

        [[nodiscard]] std::shared_ptr<neon::MutableAsset<neon::TextureView>> getPickerTexture() const;

        void setSkybox(const std::shared_ptr<neon::SampledTexture>& skybox) const;

        [[nodiscard]] bool isHovered() const;

        [[nodiscard]] ImVec2 getWindowSize() const;

        [[nodiscard]] ImVec2 getWindowOrigin() const;

        void focusScene() const;

        void onStart() override;

        void onPreDraw() override;

        void drawWindow() override;
    };
} // namespace neoneuron

#endif // NEONEURON_VIEWPORT_H
