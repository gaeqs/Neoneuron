//
// Created by gaeqs on 31/03/25.
//

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
        std::shared_ptr<neon::Material> _material;
        std::shared_ptr<neon::Model> _model;

        /**
         * Information required to create a GPUSynapse,
         * Conveniently folded for an easy use.
         */
        GPUSynapseCreateInfo _createInfo;

        hey::Listener<mindset::Synapse*> _synapseAddListener;
        hey::Listener<mindset::UID> _synapseRemoveListener;
        hey::Listener<void*> _clearListener;

        std::unordered_map<mindset::UID, GPUSynapse> _gpuSynapses;

        void loadUniformBuffers();

        void loadShader();

        void loadMaterial();

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

        void refreshNeuronProperty(mindset::UID neuronId, const std::string& propertyName) override;
    };

} // namespace neoneuron

#endif //SYNAPSEREPRESENTATION_H
