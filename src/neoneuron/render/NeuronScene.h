//
// Created by gaeqs on 9/10/24.
//

#ifndef NEURONSCENE_H
#define NEURONSCENE_H
#include <memory>

#include <neon/Neon.h>
#include <neoneuron/structure/Neuron.h>
#include <neoneuron/render/GPUNeuron.h>
#include <neoneuron/render/AbstractNeuronScene.h>
#include <neoneuron/render/NeuronSelector.h>


namespace neoneuron {
    class NeoneuronRender;

    class NeuronScene : public AbstractNeuronScene {
        NeoneuronRender* _render;
        NeuronSelector _selector;
        std::shared_ptr<neon::Model> _neuronModel;
        std::vector<Neuron> _neurons;
        std::vector<GPUNeuron> _gpuNeurons;
        rush::AABB<3, float> _sceneBoundingBox;


        void combineBoundingBoxes(const rush::AABB<3, float>& aabb);

        void recalculateBoundingBox();

    public:
        explicit NeuronScene(NeoneuronRender* render);

        ~NeuronScene() override;

        [[nodiscard]] const std::vector<Neuron>& getNeurons() const;

        [[nodiscard]] AbstractSelector& getSelector() override;

        [[nodiscard]] const AbstractSelector& getSelector() const override;

        [[nodiscard]] size_t getNeuronsAmount() override;

        [[nodiscard]] std::optional<Neuron*> findNeuron(UID uid);

        [[nodiscard]] std::optional<const Neuron*> findNeuron(UID uid) const;

        [[nodiscard]] std::optional<GPUNeuron*> findGPUNeuron(UID uid);

        [[nodiscard]] std::optional<const GPUNeuron*> findGPUNeuron(UID uid) const;

        bool addNeuron(const PrototypeNeuron& neuron) override;

        void addNeuron(const Neuron& neuron);

        void addNeuron(Neuron&& neuron);

        bool removeNeuron(UID neuronId) override;

        [[nodiscard]] rush::AABB<3, float> getSceneBoundingBox() const override;
    };
}

#endif //NEURONSCENE_H
