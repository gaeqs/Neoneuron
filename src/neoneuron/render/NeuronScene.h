//
// Created by gaeqs on 9/10/24.
//

#ifndef NEURONSCENE_H
#define NEURONSCENE_H
#include <memory>

#include <neon/Neon.h>
#include <neoneuron/structure/Neuron.h>
#include <neoneuron/render/GPUNeuron.h>

namespace neoneuron {
    class NeoneuronRender;

    class NeuronScene {
        NeoneuronRender* _render;
        std::shared_ptr<neon::Model> _neuronModel;
        std::vector<Neuron> _neurons;
        std::vector<GPUNeuron> _gpuNeurons;

        rush::AABB<3, float> _sceneBoundingBox;
        rush::Sphere<3, float> _sceneSphereBoundingBox;

    public:
        NeuronScene() = default;

        NeuronScene(const NeuronScene& other) = delete;

        NeuronScene(NeuronScene&& other) noexcept;

        NeuronScene& operator=(NeuronScene&& other) noexcept;

        explicit NeuronScene(NeoneuronRender* render);

        ~NeuronScene();

        void addNeuron(const Neuron& neuron);

        void addNeuron(Neuron&& neuron);

        void removeNeuron(UID neuronId);
    };
}

#endif //NEURONSCENE_H
