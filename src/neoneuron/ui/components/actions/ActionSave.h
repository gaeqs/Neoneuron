//
// Created by gaeqs on 14/02/2025.
//

#ifndef ACTIONSAVE_H
#define ACTIONSAVE_H
#include <neon/util/task/Coroutine.h>
#include <neoneuron/render/AbstractNeuronScene.h>
#include <neoneuron/ui/components/ModalComponent.h>

namespace neoneuron {
    class ActionSave : public ModalComponent {

        AbstractNeuronScene* _scene;

        void run();

        neon::Coroutine<> saveCoroutine(std::unordered_set<mnemea::UID> uids);

    public:
        ActionSave(AbstractNeuronScene* scene);

        void onModalDraw() override;

        void actionButton(ImVec2 recommendedSize) override;
    };
}


#endif //ACTIONSAVE_H
