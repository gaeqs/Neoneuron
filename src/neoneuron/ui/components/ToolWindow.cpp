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

//
// Created by gaeqs on 15/05/25.
//

#include "ToolWindow.h"

namespace neoneuron
{

    void ToolWindow::setWindowName(std::string windowName)
    {
        _windowName = std::move(windowName);
    }

    ToolWindow::ToolWindow(NeoneuronApplication* application, std::string configNode, std::string windowName) :
        _application(application),
        _configNode(std::move(configNode)),
        _windowName(std::move(windowName))
    {
    }

    NeoneuronApplication* ToolWindow::getNeoneuronApplication() const
    {
        return _application;
    }

    void ToolWindow::onPreDraw()
    {
        auto& s = _application->getFiles().getSettings();
        bool opened = s.value(_configNode, false);
        bool keepOpen = true;

        if (opened) {
            if (ImGui::Begin(_windowName.c_str(), &keepOpen)) {
                drawWindowContents();
            }
            ImGui::End();
        }

        if (opened && !keepOpen) {
            s[_configNode] = false;
            _application->getFiles().signalSettingsChange(_configNode);
        }
    }
} // namespace neoneuron
