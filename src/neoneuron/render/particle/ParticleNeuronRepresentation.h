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

//
// Created by gaeqs on 8/10/25.
//

#ifndef NEONEURON_ACTIVITYREPRESENTATION_H
#define NEONEURON_ACTIVITYREPRESENTATION_H

#include <neoneuron/render/AbstractNeuronRepresentation.h>
#include <neoneuron/render/NeoneuronRender.h>
#include <neoneuron/render/particle/ParticleGPUNeuron.h>
#include <neoneuron/render/extension/neuron/NeuronColorAndScaleSE.h>

namespace neoneuron
{

    struct ActivityRepresentationData
    {
        uint32_t activities;
    };

    struct CurrentEventSequence
    {
        GID activityId;
        std::string name;
        mindset::EventSequence<std::monostate> sequence;
    };

    class ParticleNeuronRepresentation : public AbstractNeuronRepresentation
    {
      public:
        static constexpr size_t UNIFORM_SET = 2;
        static constexpr size_t COLOR_AND_SCALE_SET = 3;
        static constexpr size_t REPRESENTATION_BINDING = 0;
        static constexpr size_t NEURON_BINDING = 1;

        static constexpr size_t ACTIVITY_INSTANCES = 10'000'000;

      private:
        NeoneuronRender* _render;
        rush::AABB<3, float> _sceneBoundingBox;

        std::shared_ptr<neon::ShaderUniformDescriptor> _uboDescriptor;
        std::shared_ptr<neon::ShaderUniformBuffer> _ubo;

        std::shared_ptr<NeuronColorAndScaleSE> _colorAndScale;

        std::shared_ptr<neon::ShaderProgram> _shader;
        std::shared_ptr<neon::Model> _model;
        neon::InstanceData* _instanceData;

        std::unordered_map<const Viewport*, std::shared_ptr<neon::Material>> _viewports;

        std::unordered_set<GID> _neuronsInData;
        std::unordered_map<GID, ParticleGPUNeuron> _gpuNeurons;

        void loadUniformBuffers();

        void loadShader();

        std::shared_ptr<neon::Material> loadMaterial(const Viewport* viewport) const;

        void loadModel();

        void onClear();

        void recalculateBoundingBox();

        void addNeuronToBoundingBox(const ParticleGPUNeuron& neuron);

        void updateGPURepresentationData() const;

      public:
        ParticleNeuronRepresentation(const ParticleNeuronRepresentation&) = delete;

        explicit ParticleNeuronRepresentation(NeoneuronRender* render);

        ~ParticleNeuronRepresentation() override;

        [[nodiscard]] NeoneuronRender* getRender() override;

        [[nodiscard]] const NeoneuronRender* getRender() const override;

        [[nodiscard]] rush::AABB<3, float> getSceneBoundingBox() const override;

        void refreshNeuronProperty(GID gid, const std::string& propertyName) override;

        void refreshData(const RepositoryView& view) override;

        void clearData() override;

        void addViewport(const Viewport* viewport) override;

        void removeViewport(const Viewport* viewport) override;

        bool hasViewport(const Viewport* viewport) override;

        void setViewports(const std::unordered_set<const Viewport*>& viewport) override;

        [[nodiscard]] size_t getTotalAllocatedMemory() const override;

        [[nodiscard]] size_t getAllocatedInstanceMemory() const override;

        [[nodiscard]] size_t getUsedInstanceMemory() const override;

        [[nodiscard]] float getUsedInstanceMemoryPercentage() const override;

        void setColorAndScale(std::shared_ptr<NeuronColorAndScaleSE> colorAndScale);
    };
} // namespace neoneuron

#endif // NEONEURON_ACTIVITYREPRESENTATION_H
