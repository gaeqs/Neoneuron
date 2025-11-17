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
// Created by gaeqs on 13/11/25.
//

#include "NeoneuronWindow.h"

#include <imgui.h>

namespace neoneuron
{
    void NeoneuronWindow::setWindowName(const std::string& windowName)
    {
        _windowName = windowName;
    }

    void NeoneuronWindow::initWindow(bool* open)
    {
        if (ImGui::Begin(_windowName.c_str(), open)) {
            drawWindow();
        }
        ImGui::End();
    }

    NeoneuronWindow::NeoneuronWindow(std::string windowName) :
        _windowName(std::move(windowName))
    {
    }

    void NeoneuronWindow::onPreDraw()
    {
        initWindow(nullptr);
    }
} // namespace neoneuron
