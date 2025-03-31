//
// Created by gaeqs on 15/01/2025.
//

#ifndef ACTIONSHUFFLE_H
#define ACTIONSHUFFLE_H
#include <imgui.h>
#include <random>
#include <neoneuron/application/NeoneuronApplication.h>
#include <neoneuron/ui/components/ModalComponent.h>

namespace neoneuron
{
    class ActionShuffle : public ModalComponent
    {
        NeoneuronApplication* _application;
        rush::Vec3f _center;
        float _radius;
        bool _shuffleRotation;

        std::random_device _randomDevice;
        std::mt19937 _randomGenerator;

        void shuffle(mindset::UID prop, mindset::Neuron* neuron);

        void run();

      public:
        explicit ActionShuffle(NeoneuronApplication* application);

        void onModalDraw() override;

        void actionButton(ImVec2 recommendedSize) override;
    };
} // namespace neoneuron

#endif //ACTIONSHUFFLE_H
