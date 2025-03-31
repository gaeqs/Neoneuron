//
// Created by gaeqs on 14/02/2025.
//

#ifndef ACTIONSAVE_H
#define ACTIONSAVE_H
#include <neon/util/task/Coroutine.h>
#include <neoneuron/render/AbstractNeuronRepresentation.h>
#include <neoneuron/ui/components/ModalComponent.h>

namespace neoneuron
{
    class ActionSave : public ModalComponent
    {
        AbstractNeuronRepresentation* _scene;

        void run();

        neon::Coroutine<> saveCoroutine(std::unordered_set<mindset::UID> uids);

      public:
        ActionSave(AbstractNeuronRepresentation* scene);

        void onModalDraw() override;

        void actionButton(ImVec2 recommendedSize) override;
    };
} // namespace neoneuron

#endif //ACTIONSAVE_H
