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

#include "Viewport.h"

#include "NeoneuronRender.h"
#include "Uniforms.h"
#include "neoneuron/ui/components/nodes/CameraNode.h"
#include "neoneuron/ui/style/MaterialSymbols.h"

namespace
{
    std::atomic_size_t NAME_ID_GENERATOR = 0;
} // namespace

namespace neoneuron
{

    std::string Viewport::getUniqueName()
    {
        return std::format("Viewport {}", NAME_ID_GENERATOR++);
    }

    void Viewport::loadUniforms()
    {
        auto* app = &_render->getApplication();

        std::vector bindings = {neon::ShaderUniformBinding::uniformBuffer(sizeof(Matrices))};
        _uniformDescriptor =
            std::make_shared<neon::ShaderUniformDescriptor>(app, "neoneuron:viewport_uniform_descriptor", bindings);
        _uniformBuffer =
            std::make_shared<neon::ShaderUniformBuffer>("neoneuron:viewport_uniform_buffer", _uniformDescriptor);
    }

    void Viewport::loadSelectionResolver()
    {
        // This asset collection is used to provide local information to the asset loader.
        // It has a higher priority than the application's asset collection!
        neon::AssetCollection localCollection;
        localCollection.store(_outputFrameBuffer, neon::AssetStorageMode::WEAK);
        localCollection.store(_uniformBuffer, neon::AssetStorageMode::WEAK);
        localCollection.store(_uniformDescriptor, neon::AssetStorageMode::WEAK);

        auto* application = &_render->getApplication();
        auto fileSystem = _render->getFileSystem();
        neon::AssetLoaderContext context(application, nullptr, &fileSystem);
        context.localCollection = &localCollection;

        _selectionResolver = neon::loadAssetFromFile<neon::Model>("model/selector/selector_resolver.json", context);
        _selectionResolver->getUniformBufferBindings()[2] = neon::ModelBufferBinding::extra(_uniformBuffer);

        _selectionResolver->getInstanceData(0)->createInstance();

        _render->getRoom()->markUsingModel(_selectionResolver.get());

        auto colorOutput = _inputFrameBuffer->getOutputs()[0].resolvedTexture;
        auto selectionOutput = _inputFrameBuffer->getOutputs()[1].texture;
        auto& materials = _selectionResolver->getMeshes()[0]->getMaterials();
        for (auto& material : materials) {
            auto& ubo = material->getUniformBuffer();
            ubo->setTexture(0, neon::SampledTexture::create(application, colorOutput));
            ubo->setTexture(1, neon::SampledTexture::create(application, selectionOutput));
            ubo->uploadData(3, 0);
        }
    }

    void Viewport::assignRecreationConditions(neon::SimpleFrameBuffer* frameBuffer) const
    {
        frameBuffer->setRecreationCondition([&](auto* fb) {
            auto vp = rush::Vec2i(_windowSize.x, _windowSize.y);
            if (vp.x() <= 0 || vp.y() <= 0) {
                return false;
            }
            return vp.cast<uint32_t>() != fb->getDimensions();
        });

        frameBuffer->setRecreationParameters([&](auto* app) {
            auto vp = rush::max(rush::Vec2i(_windowSize.x, _windowSize.y), 1);
            return vp.cast<uint32_t>();
        });
    }

    void Viewport::renderSidebar()
    {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
        if (ImGui::Button(ICON_MS_RECENTER)) {
            focusScene();
        }
        if (ImGui::IsItemHovered()) {
            ImGui::SetTooltip("Focus scene");
        }
        ImGui::PopStyleColor();
    }

    Viewport::Viewport(NeoneuronRender* render, int priority, const std::string& name) :
        NeoneuronWindow(name.empty() ? getUniqueName() : name),
        _render(render),
        _internalCamera(CameraNode::createDefaultCamera()),
        _externalCameraController(nullptr),
        _hovered(false)
    {
        auto* application = &render->getApplication();
        auto fileSystem = render->getFileSystem();
        auto neonRender = application->getRender();

        // Two framebuffer:
        // - One used by representations to draw its elements. 8 samples. Three textures:
        //   - neoneuron:color: R8G8B8A8. Used to draw the color. Resolved to a texture with 1 sample.
        //     - Resolved texture's name: neoneuron:resolved_color.
        //   - neoneuron:picker: R32FG32FB32FA32F. Used to store the information of the drawn element.
        //   - neoneuron:depth: D32F. Used to store the depth of the drawn elements.
        // - One used to process the data of the first framebuffer.
        //   - Transforms the picker texture into a texture with 1 sample.
        //   - Draws the background of the scene.
        //   - Same textures than the first framebuffer, but with only 1 sample and without depth.
        std::vector<neon::FrameBufferTextureCreateInfo> textures = {neon::TextureFormat::R8G8B8A8,
                                                                    neon::TextureFormat::R32FG32FB32FA32F};

        textures[0].name = "neoneuron:color";
        textures[0].resolveName = "neoneuron:resolved_color";
        textures[1].name = "neoneuron:picker";

        _inputFrameBuffer =
            std::make_shared<neon::SimpleFrameBuffer>(application, "neoneuron:input", neon::SamplesPerTexel::COUNT_8,
                                                      textures, neon::FrameBufferDepthCreateInfo("neoneuron:depth"));

        _inputFrameBuffer->setClearColor(0, rush::Vec4f(0.0f, 0.0f, 0.0f, 0.0f));

        _outputFrameBuffer = std::make_shared<neon::SimpleFrameBuffer>(application, "neoneuron:output",
                                                                       neon::SamplesPerTexel::COUNT_1, textures);

        assignRecreationConditions(_inputFrameBuffer.get());
        assignRecreationConditions(_outputFrameBuffer.get());

        _inputStrategy =
            std::make_shared<neon::DefaultRenderPassStrategy>("neoneuron:input", _inputFrameBuffer, priority);

        _outputStrategy =
            std::make_shared<neon::DefaultRenderPassStrategy>("neoneuron:output", _outputFrameBuffer, priority);

        neonRender->addRenderPass(_inputStrategy);
        neonRender->addRenderPass(_outputStrategy);

        _outputColorTexture =
            neon::SampledTexture::create(application, _outputFrameBuffer->getOutputs()[0].resolvedTexture);

        loadUniforms();
        loadSelectionResolver();
    }

    Viewport::~Viewport()
    {
        if (_internalCameraController.isValid()) {
            _internalCameraController->destroy();
        }
        _render->getRoom()->unmarkUsingModel(_selectionResolver.get());

        auto neonRender = _render->getApplication().getRender();
        neonRender->removeRenderPass(_inputStrategy);
        neonRender->removeRenderPass(_outputStrategy);
    }

    NeoneuronRender* Viewport::getRender() const
    {
        return _render;
    }

    neon::Camera& Viewport::getCamera()
    {
        if (_externalCameraController == nullptr) {
            return *_internalCamera;
        }

        return *_externalCameraController->getCamera();
    }

    const neon::Camera& Viewport::getCamera() const
    {
        if (_externalCameraController == nullptr) {
            return *_internalCamera;
        }

        return *_externalCameraController->getCamera();
    }

    neon::IdentifiableWrapper<CameraController> Viewport::getCameraController() const
    {
        if (_externalCameraController == nullptr) {
            return _internalCameraController;
        }

        return _externalCameraController;
    }

    void Viewport::setExternalCamera(neon::IdentifiableWrapper<CameraController> camera)
    {
        if (_externalCameraController == nullptr && camera != nullptr) {
            _internalCameraController->unregisterViewport(this);
        } else if (_externalCameraController != nullptr && camera == nullptr) {
            _internalCameraController->registerViewport(this);
        }

        if (_externalCameraController != nullptr) {
            _externalCameraController->unregisterViewport(this);
        }
        if (camera != nullptr) {
            camera->registerViewport(this);
        }
        _externalCameraController = camera;
    }

    CameraData& Viewport::getCameraData()
    {
        return _cameraData;
    }

    const CameraData& Viewport::getCameraData() const
    {
        return _cameraData;
    }

    std::shared_ptr<neon::SimpleFrameBuffer> Viewport::getInputFrameBuffer() const
    {
        return _inputFrameBuffer;
    }

    std::shared_ptr<neon::ShaderUniformDescriptor> Viewport::getUniformDescriptor() const
    {
        return _uniformDescriptor;
    }

    std::shared_ptr<neon::ShaderUniformBuffer> Viewport::getUniformBuffer() const
    {
        return _uniformBuffer;
    }

    std::shared_ptr<neon::MutableAsset<neon::TextureView>> Viewport::getPickerTexture() const
    {
        return _outputFrameBuffer->getOutputs()[1].resolvedTexture;
    }

    void Viewport::setSkybox(const std::shared_ptr<neon::SampledTexture>& skybox) const
    {
        auto& materials = _selectionResolver->getMeshes()[0]->getMaterials();
        if (skybox == nullptr) {
            for (auto& material : materials) {
                auto& ubo = material->getUniformBuffer();
                ubo->uploadData(3, 0);
            }
        } else {
            for (auto& material : materials) {
                auto& ubo = material->getUniformBuffer();
                ubo->setTexture(2, skybox);
                ubo->uploadData(3, 1);
            }
        }
    }

    bool Viewport::isHovered() const
    {
        return _hovered;
    }

    ImVec2 Viewport::getWindowSize() const
    {
        return _windowSize;
    }

    ImVec2 Viewport::getWindowOrigin() const
    {
        return _windowOrigin;
    }

    void Viewport::focusScene() const
    {
        bool first = true;
        rush::AABB<3, float> aabb;
        for (auto rep : _render->getRepresentations()) {
            if (rep->hasViewport(this)) {
                if (first) {
                    aabb = rep->getSceneBoundingBox();
                    first = false;
                } else {
                    auto repAABB = rep->getSceneBoundingBox();
                    auto min = rush::min(aabb.center - aabb.radius, repAABB.center - repAABB.radius);
                    auto max = rush::max(aabb.center + aabb.radius, repAABB.center + repAABB.radius);
                    aabb = rush::AABB<3, float>::fromEdges(min, max);
                }
            }
        }
        if (!first) {
            getCameraController()->focusOn(aabb);
        }
    }

    void Viewport::onStart()
    {
        _internalCameraController = CameraNode::createDefaultCameraController(getGameObject(), _internalCamera.get());
        _internalCameraController->registerViewport(this);
        _texturePicker = getGameObject()->newComponent<NeuronTexturePicker>(_render->getNeoneuronApplication(), this);
    }

    void Viewport::onPreDraw()
    {
        ImGui::SetNextWindowSizeConstraints(ImVec2(200, 200), ImVec2(100000, 100000));
        NeoneuronWindow::onPreDraw();
    }

    void Viewport::drawWindow()
    {
        auto& camera = getCamera();

        if (_windowSize.x > 0 && _windowSize.y > 0) {
            camera.setFrustum(camera.getFrustum().withAspectRatio(_windowSize.x / _windowSize.y));
        }

        // Update camera information
        Matrices matrices = {
            camera.getView(),   camera.getViewProjection(),    camera.getFrustum().getInverseProjection(),
            camera.getPlanes(), camera.getFrustum().getNear(), camera.getFrustum().getFar(),
        };

        renderSidebar();
        _windowSize = ImGui::GetContentRegionAvail();
        _windowOrigin = ImGui::GetCursorScreenPos();
        ImGui::Image(_outputColorTexture->getImGuiDescriptor(), _windowSize);
        _hovered = ImGui::IsItemHovered();

        _uniformBuffer->uploadData(0, &matrices, sizeof(Matrices));
    }
} // namespace neoneuron