//
// Created by gaeqs on 9/10/24.
//

#ifndef NEURONSCENE_H
#define NEURONSCENE_H
#include <memory>

#include <neon/Neon.h>
#include <neoneuron/structure/Neuron.h>
#include <neoneuron/render/GPUNeuron.h>

#include "AbstractNeuronScene.h"

namespace neoneuron {
    class NeoneuronRender;

    class NeuronScene : public AbstractNeuronScene {
        NeoneuronRender* _render;
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

        [[nodiscard]] size_t getNeuronsAmount() override;

        [[nodiscard]] std::optional<Neuron*> findNeuron(UID uid);

        [[nodiscard]] std::optional<const Neuron*> findNeuron(UID uid) const;

        bool addNeuron(const PrototypeNeuron& neuron) override;

        void addNeuron(const Neuron& neuron);

        void addNeuron(Neuron&& neuron);

        bool removeNeuron(UID neuronId) override;

        [[nodiscard]] rush::AABB<3, float> getSceneBoundingBox() const override;
    };
}

#endif //NEURONSCENE_H
