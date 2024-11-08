//
// Created by gaeqs on 9/10/24.
//

#include "NeoneuronUI.h"

#include <neon/util/component/DebugOverlayComponent.h>
#include <neon/util/component/DockSpaceComponent.h>
#include <neon/util/component/ViewportComponent.h>
#include <neoneuron/render/NeoneuronRender.h>
#include <neoneuron/ui/NeoneuronTopBar.h>
#include <neoneuron/ui/style/Themes.h>

namespace neoneuron {
    NeoneuronUI::NeoneuronUI(NeoneuronRender* render) {
        StyleColorsDark();
        ImGui::GetIO().ConfigWindowsMoveFromTitleBarOnly = true;
        _gameObject = render->getRoom()->newGameObject();
        _gameObject->setName("UI");
        _gameObject->newComponent<neon::DockSpaceComponent>(true);
        _viewport = _gameObject->newComponent<neon::ViewportComponent>();
        _gameObject->newComponent<neon::DebugOverlayComponent>(false, 100);
        _gameObject->newComponent<NeoneuronTopBar>(render);
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

    neon::IdentifiableWrapper<neon::ViewportComponent> NeoneuronUI::getViewport() const {
        return _viewport;
    }
}
