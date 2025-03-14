//
// Created by gaeqs on 15/01/2025.
//

#ifndef ACTIONSHUFFLE_H
#define ACTIONSHUFFLE_H
#include <imgui.h>
#include <random>
#include <neoneuron/render/AbstractNeuronScene.h>
#include <neoneuron/ui/components/ModalComponent.h>

namespace neoneuron {
    class ActionShuffle : public ModalComponent {
        AbstractNeuronScene* _scene;
        rush::Vec3f _center;
        float _radius;
        bool _shuffleRotation;

        std::random_device _randomDevice;
        std::mt19937 _randomGenerator;

        void shuffle(mnemea::UID prop, mnemea::Neuron* neuron);

        void run();

    public:
        ActionShuffle(AbstractNeuronScene* scene);

        void onModalDraw() override;

        void actionButton(ImVec2 recommendedSize) override;
    };
}


#endif //ACTIONSHUFFLE_H
