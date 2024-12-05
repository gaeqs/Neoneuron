//
// Created by gaeqs on 9/10/24.
//

#include "NeoneuronUI.h"

#include <neon/util/component/DebugOverlayComponent.h>
#include <neon/util/component/DockSpaceComponent.h>
#include <neon/util/component/ViewportComponent.h>
#include <neoneuron/application/NeoneuronApplication.h>
#include <neoneuron/render/NeoneuronRender.h>
#include <neoneuron/ui/NeoneuronTopBar.h>
#include <neoneuron/ui/style/Themes.h>
#include <neoneuron/ui/style/Fonts.h>

#include <neoneuron/ui/components/NeoneuronUINeuronList.h>
#include <neoneuron/ui/components/NeoneuronUIGlobalParameters.h>
#include <neoneuron/ui/components/NeuronTexturePicker.h>

namespace neoneuron {
    void NeoneuronUI::initStyle(NeoneuronRender* render) {
        switch (render->getNeoneuronApplication()->getSettings().value(NeoneuronApplication::SETTINGS_THEME, 0)) {
            case 0:
                StyleColorsDark();
                break;
            case 1:
                StyleColorsLight();
                break;
            default:
                break;
        }
    }

    void NeoneuronUI::initDebugToggle() {
        _debugKeyListener = [this](std::string data) {
            if (data != NeoneuronApplication::SETTINGS_TOOL_DEBUG) return;
            if (!_gameObject.isValid()) return;
            bool value = _render->getNeoneuronApplication()->getSettings()
                    .value(NeoneuronApplication::SETTINGS_TOOL_DEBUG, false);
            auto component = _gameObject->findComponent<neon::DebugOverlayComponent>();
            if (component.isValid() == value) return;
            if (value) {
                _gameObject->newComponent<neon::DebugOverlayComponent>(false, 100);
            } else {
                component->destroyLater();
            }
        };

        _render->getNeoneuronApplication()->registerSettingsListener(_debugKeyListener);

        bool value = _render->getNeoneuronApplication()->getSettings()
                .value(NeoneuronApplication::SETTINGS_TOOL_DEBUG, false);
        if (value) {
            _gameObject->newComponent<neon::DebugOverlayComponent>(false, 100);
        }
    }

    NeoneuronUI::NeoneuronUI(NeoneuronRender* render) : _render(render) {
        initStyle(render);
        ImGui::GetIO().ConfigWindowsMoveFromTitleBarOnly = true;
        _gameObject = render->getRoom()->newGameObject();
        _gameObject->setName("UI");
        _gameObject->newComponent<neon::DockSpaceComponent>(true);
        _viewport = _gameObject->newComponent<neon::ViewportComponent>();
        _gameObject->newComponent<neon::LogComponent>();
        _gameObject->newComponent<NeoneuronTopBar>(render);
        _gameObject->newComponent<NeoneuronUINeuronList>(render);
        _gameObject->newComponent<NeoneuronUIGlobalParameters>(render);
        _gameObject->newComponent<NeuronTexturePicker>(render, _viewport);

        initDebugToggle();

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
            _render->getNeoneuronApplication()->unregisterSettingsListener(_debugKeyListener);
        }
    }

    NeoneuronUI& NeoneuronUI::operator=(NeoneuronUI&& other) noexcept {
        _render = other._render;
        _gameObject = other._gameObject;
        _viewport = other._viewport;
        _debugKeyListener = std::move(other._debugKeyListener);
        other._gameObject = nullptr;
        return *this;
    }

    neon::IdentifiableWrapper<neon::ViewportComponent> NeoneuronUI::getViewport() const {
        return _viewport;
    }
}
