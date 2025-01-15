//
// Created by gaeqs on 15/01/2025.
//

#ifndef MODALCOMPONENT_H
#define MODALCOMPONENT_H

#include <imgui.h>
#include <neon/structure/Component.h>

namespace neoneuron {
    class ModalComponent : public neon::Component {
        std::string _name;
        bool _hasActionButton;

        bool _open;

    public:
        ModalComponent(std::string name, bool hasActionButton);

        void onPreDraw() override;

        virtual void onModalDraw() = 0;

        virtual void actionButton(ImVec2 recommendedSize) = 0;
    };
}


#endif //MODALCOMPONENT_H
