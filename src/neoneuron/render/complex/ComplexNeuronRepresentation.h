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

#ifndef NEONEURON_COMPLEXNEURONSCENE_H
#define NEONEURON_COMPLEXNEURONSCENE_H

#include <memory>
#include <unordered_map>

#include <hey/Listener.h>

#include <neon/Neon.h>
#include <neoneuron/structure/complex/ComplexNeuron.h>
#include <neoneuron/render/complex/ComplexGPUNeuron.h>
#include <neoneuron/render/AbstractNeuronRepresentation.h>
#include <neoneuron/render/ChangeProcessor.h>
#include <neoneuron/structure/Selector.h>

namespace neoneuron
{
    class NeoneuronRender;

    struct ComplexGPUSelectionData
    {
        uint32_t selected;
    };

    struct ComplexNeuronRepresentationData
    {
        uint32_t sections;
        uint32_t joints;
    };

    struct ComplexNeuronRepresentations
    {
        ComplexNeuron neuron;
        ComplexGPUNeuron gpu;
    };

    struct ComplexNeuronProcessData
    {
        GID gid;
        mindset::Dataset* dataset;
        mindset::Neuron* neuron;
    };

    class ComplexNeuronRepresentation : public AbstractNeuronRepresentation
    {
      public:
        static constexpr size_t UNIFORM_SET = 2;
        static constexpr size_t REPRESENTATION_BINDING = 0;
        static constexpr size_t NEURON_BINDING = 1;
        static constexpr size_t SEGMENT_BINDING = 2;
        static constexpr size_t JOINT_BINDING = 3;
        static constexpr size_t SOMA_BINDING = 4;
        static constexpr size_t SELECTION_BINDING = 5;
        static constexpr size_t SOMA_GPU_DATA_BINDING = 6;
        static constexpr size_t SECTION_GPU_DATA_BINDING = 7;
        static constexpr size_t SAVING_VERTICES_DATA_BINDING = 8;
        static constexpr size_t SAVING_INDICES_DATA_BINDING = 9;

        static constexpr size_t SEGMENT_INSTANCES = 10000000;
        static constexpr size_t JOINT_INSTANCES = 1000000;
        static constexpr size_t SOMA_INSTANCES = 100000;
        static constexpr size_t STORAGE_PER_SOMA = 64 * 64;
        static constexpr size_t STORAGE_PER_SECTION = sizeof(uint32_t);
        static constexpr size_t MAX_SAVE_VERTICES = 50000000;

        struct Materials
        {
            std::shared_ptr<neon::Material> segment;
            std::shared_ptr<neon::Material> joint;
            std::shared_ptr<neon::Material> soma;
        };

      private:
        NeoneuronRender* _render;

        std::shared_ptr<neon::ShaderUniformDescriptor> _uboDescriptor;
        std::shared_ptr<neon::ShaderUniformBuffer> _ubo;
        std::shared_ptr<neon::StorageBufferInstanceData> _globalInstanceData;

        std::shared_ptr<neon::ShaderProgram> _neuronShader;
        std::shared_ptr<neon::ShaderProgram> _jointShader;
        std::shared_ptr<neon::ShaderProgram> _somaShader;

        std::unordered_map<Viewport*, Materials> _viewports;

        std::shared_ptr<neon::Model> _segmentModel;
        std::shared_ptr<neon::Model> _jointModel;
        std::shared_ptr<neon::Model> _somaModel;

        ChangeProcessor<ComplexNeuronProcessData, ComplexNeuron> _neuronProcessor;

        hey::Listener<SelectionEvent> _selectionListener;
        hey::Listener<GID> _processorListener;

        std::unordered_set<GID> _neuronsInDataset;
        std::unordered_map<GID, ComplexNeuronRepresentations> _neurons;
        rush::AABB<3, float> _sceneBoundingBox;

        std::vector<uint32_t> _selection;

        bool _wireframe;
        bool _drawSegments;
        bool _drawJoints;
        bool _drawSomas;

        void loadUniformBuffers();

        void loadNeuronShader();

        void loadJointShader();

        void loadSomaShader();

        std::shared_ptr<neon::Material> loadSegmentMaterial(const Viewport* viewport) const;

        std::shared_ptr<neon::Material> loadJointMaterial(const Viewport* viewport) const;

        std::shared_ptr<neon::Material> loadSomaMaterial(const Viewport* viewport) const;

        void loadNeuronModel();

        void loadJointModel();

        void loadSomaModel();

        void combineBoundingBoxes(const rush::AABB<3, float>& aabb);

        void recalculateBoundingBox();

        void recreateMaterials();

        void postProcess();

        void addComplexNeuron(ComplexNeuron&& complex);

        void onSelectionEvent(SelectionEvent event);

        void refreshSelectionData();

        void updateGPURepresentationData() const;

        void removeMaterials(const Materials& materials);

      public:
        ComplexNeuronRepresentation(const ComplexNeuronRepresentation&) = delete;

        explicit ComplexNeuronRepresentation(NeoneuronRender* render);

        ~ComplexNeuronRepresentation() override;

        [[nodiscard]] NeoneuronRender* getRender() override;

        [[nodiscard]] const NeoneuronRender* getRender() const override;

        [[nodiscard]] size_t getNeuronsAmount() const;

        [[nodiscard]] size_t getSectionsAmount() const;

        [[nodiscard]] size_t getJointsAmount() const;

        [[nodiscard]] size_t getSomasAmount() const;

        [[nodiscard]] std::optional<ComplexNeuron*> findNeuron(GID gid);

        [[nodiscard]] std::optional<const ComplexNeuron*> findNeuron(GID gid) const;

        [[nodiscard]] std::optional<ComplexGPUNeuron*> findGPUNeuron(GID gid);

        [[nodiscard]] std::optional<const ComplexGPUNeuron*> findGPUNeuron(GID gid) const;

        [[nodiscard]] rush::AABB<3, float> getSceneBoundingBox() const override;

        [[nodiscard]] bool shouldDrawSegments() const;

        void setDrawSegments(bool draw);

        [[nodiscard]] bool shouldDrawJoints() const;

        void setDrawJoints(bool draw);

        [[nodiscard]] bool shouldDrawSomas() const;

        void setDrawSomas(bool draw);

        const std::shared_ptr<neon::ShaderUniformBuffer>& getUBO() const;

        void refreshNeuronProperty(GID neuronId, const std::string& propertyName) override;

        void refreshData(const RepositoryView& view) override;

        void clearData() override;

        void addViewport(Viewport* viewport) override;

        void removeViewport(Viewport* viewport) override;

        void setViewports(const std::unordered_set<Viewport*>& viewport) override;

        [[nodiscard]] size_t getTotalAllocatedMemory() const override;

        [[nodiscard]] size_t getAllocatedInstanceMemory() const override;

        [[nodiscard]] size_t getUsedInstanceMemory() const override;

        [[nodiscard]] float getUsedInstanceMemoryPercentage() const override;

        [[nodiscard]] bool isWireframeMode() const;

        void setWireframeMode(bool wireframe);

        void reloadShader();
    };
} // namespace neoneuron

#endif // NEONEURON_COMPLEXNEURONSCENE_H
