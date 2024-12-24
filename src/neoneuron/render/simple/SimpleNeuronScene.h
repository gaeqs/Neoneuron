//
// Created by gaeqs on 9/10/24.
//

#ifndef NEURONSCENE_H
#define NEURONSCENE_H
#include <memory>

#include <neon/Neon.h>
#include <neoneuron/structure/simple/SimpleNeuron.h>
#include <neoneuron/render/simple/SimpleGPUNeuron.h>
#include <neoneuron/render/AbstractNeuronScene.h>
#include <neoneuron/render/simple/SimpleNeuronSelector.h>


namespace neoneuron {
    class NeoneuronRender;

    class SimpleNeuronScene : public AbstractNeuronScene {
        NeoneuronRender* _render;
        SimpleNeuronSelector _selector;
        std::shared_ptr<neon::Model> _neuronModel;
        std::vector<SimpleNeuron> _neurons;
        std::vector<SimpleGPUNeuron> _gpuNeurons;
        rush::AABB<3, float> _sceneBoundingBox;


        void combineBoundingBoxes(const rush::AABB<3, float>& aabb);

        void recalculateBoundingBox();

    public:
        explicit SimpleNeuronScene(NeoneuronRender* render);

        ~SimpleNeuronScene() override;

        [[nodiscard]] const std::vector<SimpleNeuron>& getNeurons() const;

        [[nodiscard]] AbstractSelector& getSelector() override;

        [[nodiscard]] const AbstractSelector& getSelector() const override;

        [[nodiscard]] size_t getNeuronsAmount() override;

        [[nodiscard]] std::optional<SimpleNeuron*> findNeuron(UID uid);

        [[nodiscard]] std::optional<const SimpleNeuron*> findNeuron(UID uid) const;

        [[nodiscard]] std::optional<SimpleGPUNeuron*> findGPUNeuron(UID uid);

        [[nodiscard]] std::optional<const SimpleGPUNeuron*> findGPUNeuron(UID uid) const;

        bool addNeuron(const PrototypeNeuron& neuron) override;

        bool addNeuron(PrototypeNeuron&& neuron) override;

        void addNeuron(const SimpleNeuron& neuron);

        void addNeuron(SimpleNeuron&& neuron);

        bool removeNeuron(UID neuronId) override;

        [[nodiscard]] rush::AABB<3, float> getSceneBoundingBox() const override;
    };
}

#endif //NEURONSCENE_H
