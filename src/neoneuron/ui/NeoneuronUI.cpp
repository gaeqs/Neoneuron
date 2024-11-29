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
#include <neoneuron/ui/style/Fonts.h>

#include <neoneuron/ui/components/NeoneuronUINeuronList.h>
#include <neoneuron/ui/components/NeoneuronUIGlobalParameters.h>
#include <neoneuron/ui/components/NeuronTexturePicker.h>

namespace neoneuron {
    NeoneuronUI::NeoneuronUI(NeoneuronRender* render) {
        StyleColorsDark();
        ImGui::GetIO().ConfigWindowsMoveFromTitleBarOnly = true;
        _gameObject = render->getRoom()->newGameObject();
        _gameObject->setName("UI");
        _gameObject->newComponent<neon::DockSpaceComponent>(true);
        _viewport = _gameObject->newComponent<neon::ViewportComponent>();
        _gameObject->newComponent<neon::DebugOverlayComponent>(false, 100);
        _gameObject->newComponent<neon::LogComponent>();
        _gameObject->newComponent<NeoneuronTopBar>(render);
        _gameObject->newComponent<NeoneuronUINeuronList>(render);
        _gameObject->newComponent<NeoneuronUIGlobalParameters>(render);
        _gameObject->newComponent<NeuronTexturePicker>(render, _viewport);

        auto& fs = render->getFileSystem();
        auto file = fs.readFile("/font/SourceSans3.ttf");
        if (file.has_value()) {
            fonts::loadFont(fonts::SS3_16, file.value(), 16.0f);
            fonts::loadFont(fonts::SS3_18, file.value(), 18.0f);
            fonts::loadFont(fonts::SS3_20, file.value(), 20.0f);
            fonts::loadFont(fonts::SS3_24, file.value(), 24.0f);
            fonts::loadFont(fonts::SS3_32, file.value(), 32.0f);

            if (auto font = fonts::getFont(fonts::SS3_18); font.has_value()) {
                ImGui::GetIO().FontDefault = font.value();
            }
        }

        fonts::recreateFonts();
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
