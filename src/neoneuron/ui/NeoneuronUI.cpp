// Copyright (c) 2025. VG-Lab/URJC.
//
// Authors: Gael Rial Costas <gael.rial.costas@urjc.es>
//
// This file is part of Neoneuron <gitlab.gmrv.es/g.rial/neoneuron>
//
// This library is free software; you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License version 3.0 as published
// by the Free Software Foundation.
//
// This library is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this library; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

#include "NeoneuronUI.h"

#include "components/NeoneuronBottomBar.h"
#include "components/NeoneuronUINodeEditor.h"
#include "components/NeoneuronUIPerformanceRecorder.h"
#include "style/MaterialSymbols.h"

#include <neon/util/component/DebugOverlayComponent.h>
#include <neon/util/component/DockSpaceComponent.h>
#include <neoneuron/application/NeoneuronApplication.h>
#include <neoneuron/render/NeoneuronRender.h>
#include <neoneuron/ui/NeoneuronTopBar.h>
#include <neoneuron/ui/style/Themes.h>
#include <neoneuron/ui/style/Fonts.h>

#include <neoneuron/ui/components/NeoneuronUINeuronList.h>
#include <neoneuron/ui/components/NeoneuronUIGlobalParameters.h>
#include <vulkan/util/component/VulkanInfoCompontent.h>

namespace neoneuron
{
    void NeoneuronUI::initStyle(NeoneuronRender* render)
    {
        auto& s = render->getNeoneuronApplication()->getFiles().getSettings();
        switch (s.value(NeoneuronFiles::SETTINGS_THEME, 0)) {
            case 0:
                StyleColorsDark();
                break;
            case 1:
                StyleColorsLight();
                break;
            default:
                break;
        }

        const char* font = fonts::SS3_18;
        switch (s.value(NeoneuronFiles::SETTINGS_FONT_SIZE, 1)) {
            case 0:
                font = fonts::SS3_16;
                break;
            case 1:
                font = fonts::SS3_18;
                break;
            case 2:
                font = fonts::SS3_20;
                break;
            case 3:
                font = fonts::SS3_24;
                break;
            case 4:
                font = fonts::SS3_32;
                break;
            default:
                break;
        }

        if (auto opt = fonts::getFont(font); opt.has_value()) {
            ImGui::GetIO().FontDefault = opt.value();
        }
    }

    void NeoneuronUI::initDebugToggle()
    {
        _debugKeyListener = [this](std::string data) {
            if (data != NeoneuronFiles::SETTINGS_TOOL_DEBUG) {
                return;
            }
            if (!_gameObject.isValid()) {
                return;
            }
            bool value = _render->getNeoneuronApplication()->getFiles().getSettings().value(
                NeoneuronFiles::SETTINGS_TOOL_DEBUG, false);
            auto component = _gameObject->findComponent<neon::DebugOverlayComponent>();
            if (component.isValid() == value) {
                return;
            }
            if (value) {
                _gameObject->newComponent<neon::DebugOverlayComponent>(false, 100);
            } else {
                component->destroyLater();
            }
        };

        auto& files = _render->getNeoneuronApplication()->getFiles();
        files.registerSettingsListener(_debugKeyListener);
        if (files.getSettings().value(NeoneuronFiles::SETTINGS_TOOL_DEBUG, false)) {
            _gameObject->newComponent<neon::DebugOverlayComponent>(false, 100);
        }
    }

    NeoneuronUI::NeoneuronUI(NeoneuronRender* render) :
        _render(render)
    {
        auto* app = render->getNeoneuronApplication();
        ImGui::GetIO().ConfigWindowsMoveFromTitleBarOnly = true;
        _gameObject = render->getRoom()->newGameObject();
        _gameObject->setName("UI");
        _gameObject->newComponent<neon::DockSpaceComponent>(true);
        _gameObject->newComponent<neon::LogComponent>();
        _gameObject->newComponent<NeoneuronTopBar>(render);
        _gameObject->newComponent<NeoneuronBottomBar>(app);
        _gameObject->newComponent<NeoneuronUINeuronList>(app);
        _gameObject->newComponent<NeoneuronUIGlobalParameters>(app);
        _gameObject->newComponent<NeoneuronUINodeEditor>(app);
        _gameObject->newComponent<NeoneuronUIPerformanceRecorder>(app);

        initDebugToggle();

        auto& fs = render->getFileSystem();

        auto icons = fs.readFile("/font/MaterialSymbols.ttf");
        if (icons.has_value()) {
            fonts::addMergeFont("material", std::move(icons.value()), {ICON_MIN_MS, ICON_MAX_MS}, {0, 3});
        }

        auto file = fs.readFile("/font/SourceSans3.ttf");
        if (file.has_value()) {
            fonts::loadFont(fonts::SS3_16, file.value(), 16.0f);
            fonts::loadFont(fonts::SS3_18, file.value(), 18.0f);
            fonts::loadFont(fonts::SS3_20, file.value(), 20.0f);
            fonts::loadFont(fonts::SS3_24, file.value(), 24.0f);
            fonts::loadFont(fonts::SS3_32, file.value(), 32.0f);
        }
        fonts::recreateFonts();

        initStyle(render);
    }

    NeoneuronUI::~NeoneuronUI()
    {
        if (_gameObject.isValid()) {
            _gameObject->destroy();
            _render->getNeoneuronApplication()->getFiles().unregisterSettingsListener(_debugKeyListener);
        }
    }

    NeoneuronUI& NeoneuronUI::operator=(NeoneuronUI&& other) noexcept
    {
        _render = other._render;
        _gameObject = other._gameObject;
        _debugKeyListener = std::move(other._debugKeyListener);
        other._gameObject = nullptr;
        return *this;
    }
} // namespace neoneuron
