//
// Created by gaeqs on 9/10/24.
//

#include "NeoneuronUI.h"

#include <neon/util/component/DockSpaceComponent.h>
#include <neon/util/component/ViewportComponent.h>

namespace neoneuron {
    NeoneuronUI::NeoneuronUI(neon::Room* room) {
        _gameObject = room->newGameObject();
        _gameObject->newComponent<neon::DockSpaceComponent>();
        _gameObject->newComponent<neon::ViewportComponent>();
    }

    NeoneuronUI::~NeoneuronUI() {
        _gameObject->destroy();
    }
}
