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

#ifndef SYNAPSEREPRESENTATION_H
#define SYNAPSEREPRESENTATION_H

#include <neon/render/model/Model.h>
#include <neon/render/shader/Material.h>
#include <neon/render/shader/ShaderProgram.h>
#include <neoneuron/render/AbstractNeuronRepresentation.h>
#include <neoneuron/render/synapse/GPUSynapse.h>

namespace neoneuron
{

    struct SynapseRepresentationData
    {
        uint32_t synapses;
    };

    class SynapseRepresentation : public AbstractNeuronRepresentation
    {
      public:
        static constexpr size_t UNIFORM_SET = 2;
        static constexpr size_t REPRESENTATION_BINDING = 0;
        static constexpr size_t SYNAPSE_BINDING = 1;

        static constexpr size_t SYNAPSE_INSTANCES = 10'000'000;

      private:
        NeoneuronRender* _render;
        rush::AABB<3, float> _sceneBoundingBox;

        std::shared_ptr<neon::ShaderUniformDescriptor> _uboDescriptor;
        std::shared_ptr<neon::ShaderUniformBuffer> _ubo;

        std::shared_ptr<neon::ShaderProgram> _shader;
        std::shared_ptr<neon::Model> _model;

        /**
         * Information required to create a GPUSynapse,
         * Conveniently folded for an easy use.
         */
        GPUSynapseCreateInfo _createInfo;

        std::unordered_map<Viewport*, std::shared_ptr<neon::Material>> _viewports;

        std::unordered_set<GID> _synapsesInDataset;
        std::unordered_map<GID, GPUSynapse> _gpuSynapses;

        void loadUniformBuffers();

        void loadShader();

        std::shared_ptr<neon::Material> loadMaterial(const Viewport* viewport) const;

        void loadModel();

        void onSynapseAdded(mindset::Synapse* synapse);

        void onSynapseRemoved(mindset::UID uid);

        void onClear();

        void recalculateBoundingBox();

        void addSynapseToBoundingBox(const GPUSynapse& synapse);

        void updateGPURepresentationData() const;

      public:
        SynapseRepresentation(const SynapseRepresentation&) = delete;

        explicit SynapseRepresentation(NeoneuronRender* render);

        ~SynapseRepresentation() override;

        [[nodiscard]] NeoneuronRender* getRender() override;

        [[nodiscard]] const NeoneuronRender* getRender() const override;

        [[nodiscard]] rush::AABB<3, float> getSceneBoundingBox() const override;

        void refreshNeuronProperty(GID gid, const std::string& propertyName) override;

        void refreshData(const RepositoryView& view) override;

        void clearData() override;

        void addViewport(Viewport* viewport) override;

        void removeViewport(Viewport* viewport) override;

        void setViewports(const std::unordered_set<Viewport*>& viewport) override;

        [[nodiscard]] size_t getTotalAllocatedMemory() const override;

        [[nodiscard]] size_t getAllocatedInstanceMemory() const override;

        [[nodiscard]] size_t getUsedInstanceMemory() const override;

        [[nodiscard]] float getUsedInstanceMemoryPercentage() const override;
    };

} // namespace neoneuron
#endif // SYNAPSEREPRESENTATION_H
