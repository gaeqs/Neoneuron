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
// Created by gaeqs on 6/10/25.
//

#include "Timeline.h"

#include "implot.h"

namespace neoneuron
{

    void Timeline::recalculateTimelineSize()
    {
        if (_timeAwares.empty()) {
            _minTime = 0.0f;
            _maxTime = 0.0f;
        }

        auto it = _timeAwares.begin();
        auto end = _timeAwares.end();

        _minTime = it->get()->getStartTime();
        _maxTime = it->get()->getEndTime();

        ++it;
        for (; it != end; ++it) {
            _minTime = std::min(_minTime, it->get()->getStartTime());
            _maxTime = std::max(_maxTime, it->get()->getEndTime());
        }
    }

    Timeline::Timeline() :
        _minTime(0.0f),
        _maxTime(0.0f)
    {
    }

    void Timeline::addTimeAware(std::shared_ptr<TimeAware> timeAware)
    {
        if (auto [it, ok] = _timeAwares.insert(std::move(timeAware)); ok) {
            recalculateTimelineSize();
        }
    }

    void Timeline::removeTimeAware(std::shared_ptr<TimeAware> timeAware)
    {
        if (_timeAwares.erase(std::move(timeAware))) {
            recalculateTimelineSize();
        }
    }

    void Timeline::onPreDraw()
    {
        ImPlot::ShowDemoWindow();
        if (ImGui::Begin("Timeline")) {
            float data[] = {2.0f, 5.0f, 3.0f, 1.0f, 5.0f, 2.0f, 5.0f, 3.0f, 1.0f, 2.0f, 5.0f, 3.0f, 1.0f};
            float data2[] = {2.0f, 5.0f, 3.0f, 1.0f, 5.0f, 7.0f, 9.0f, 3.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f};
            ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.25f);
            if (ImPlot::BeginPlot("Test", ImVec2(-1, -1), ImPlotFlags_NoInputs | ImPlotFlags_NoTitle)) {
                ImPlot::PlotShaded("Test", data, sizeof(data) / sizeof(float), 0, 1, 0, ImPlotLineFlags_Shaded);
                ImPlot::PlotShaded("Test2", data2, sizeof(data2) / sizeof(float), 0, 1, 0, ImPlotLineFlags_Shaded);
                ImPlot::EndPlot();
            }
            ImPlot::PopStyleVar();
        }
        ImGui::End();
    }

} // namespace neoneuron
