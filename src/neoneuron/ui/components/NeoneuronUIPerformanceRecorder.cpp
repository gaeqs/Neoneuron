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
// Created by gaeqs on 27/6/25.
//

#include "NeoneuronUIPerformanceRecorder.h"

#include "neoneuron/ui/style/MaterialSymbols.h"

namespace neoneuron
{
    void NeoneuronUIPerformanceRecorder::drawWindowContents()
    {
        if (ImGui::Checkbox("Enabled", &_enabled)) {
            _output.flush();
        }
    }

    NeoneuronUIPerformanceRecorder::NeoneuronUIPerformanceRecorder(NeoneuronApplication* application) :
        ToolWindow(application, NeoneuronFiles::SETTINGS_TOOL_PERFORMANCE,
                   ICON_MS_PERFORMANCE_MAX "Performance Recorder"),
        _enabled(false),
        _output("performance.csv", std::ios::app)
    {
    }

    NeoneuronUIPerformanceRecorder::~NeoneuronUIPerformanceRecorder()
    {
        _output.close();
    }

    void NeoneuronUIPerformanceRecorder::onUpdate(float deltaTime)
    {
        if (_enabled) {
            double delay = getApplication()->getCurrentFrameInformation().lastFrameProcessTime;
            _output << (1 / delay) << "\n";
        }
    }

} // namespace neoneuron