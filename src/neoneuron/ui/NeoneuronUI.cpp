//
// Created by gaeqs on 9/10/24.
//

#include "NeoneuronUI.h"

#include <neon/util/component/DebugOverlayComponent.h>
#include <neon/util/component/DockSpaceComponent.h>
#include <neon/util/component/ViewportComponent.h>

namespace neoneuron {
    NeoneuronUI::NeoneuronUI(neon::Room* room) {
        _gameObject = room->newGameObject();
        _gameObject->newComponent<neon::DockSpaceComponent>();
        _gameObject->newComponent<neon::ViewportComponent>();
        _gameObject->newComponent<neon::DebugOverlayComponent>(false, 100);
    }

    NeoneuronUI::~NeoneuronUI() {
        if (_gameObject.isValid()) {
            _gameObject->destroy();
        }
    }

    NeoneuronUI& NeoneuronUI::operator=(NeoneuronUI&& other) noexcept {
        _gameObject = other._gameObject;
        other._gameObject = nullptr;
        return *this;
    }
}
