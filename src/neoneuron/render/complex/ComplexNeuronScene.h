//
// Created by gaeqs on 9/10/24.
//

#ifndef COMPLEXNEURONSCENE_H
#define COMPLEXNEURONSCENE_H
#include <memory>

#include <neon/Neon.h>
#include <neoneuron/structure/complex/ComplexNeuron.h>
#include <neoneuron/render/complex/ComplexGPUNeuron.h>
#include <neoneuron/render/AbstractNeuronScene.h>
#include <neoneuron/render/complex/ComplexNeuronSelector.h>


namespace neoneuron {
    class NeoneuronRender;

    class ComplexNeuronScene : public AbstractNeuronScene {
        NeoneuronRender* _render;
        ComplexNeuronSelector _selector;

        std::shared_ptr<neon::ShaderUniformDescriptor> _uboDescriptor;
        std::shared_ptr<neon::ShaderUniformBuffer> _ubo;

        std::shared_ptr<neon::Model> _neuronModel;
        std::shared_ptr<neon::Model> _jointModel;
        std::vector<ComplexNeuron> _neurons;
        std::vector<ComplexGPUNeuron> _gpuNeurons;
        rush::AABB<3, float> _sceneBoundingBox;

        void loadUniformBuffers();

        void loadNeuronModel();

        void loadJointModel();

        void combineBoundingBoxes(const rush::AABB<3, float>& aabb);

        void recalculateBoundingBox();

    public:
        explicit ComplexNeuronScene(NeoneuronRender* render);

        ~ComplexNeuronScene() override;

        [[nodiscard]] const std::vector<ComplexNeuron>& getNeurons() const;

        [[nodiscard]] AbstractSelector& getSelector() override;

        [[nodiscard]] const AbstractSelector& getSelector() const override;

        [[nodiscard]] size_t getNeuronsAmount() override;

        [[nodiscard]] std::optional<ComplexNeuron*> findNeuron(UID uid);

        [[nodiscard]] std::optional<const ComplexNeuron*> findNeuron(UID uid) const;

        [[nodiscard]] std::optional<ComplexGPUNeuron*> findGPUNeuron(UID uid);

        [[nodiscard]] std::optional<const ComplexGPUNeuron*> findGPUNeuron(UID uid) const;

        bool addNeuron(const PrototypeNeuron& neuron) override;

        void addNeuron(const ComplexNeuron& neuron);

        void addNeuron(ComplexNeuron&& neuron);

        bool removeNeuron(UID neuronId) override;

        [[nodiscard]] rush::AABB<3, float> getSceneBoundingBox() const override;
    };
}

#endif //COMPLEXNEURONSCENE_H
