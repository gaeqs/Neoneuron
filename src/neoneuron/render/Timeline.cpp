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
#include <imgui_internal.h>

#include <neon/util/ImGuiUtils.h>
#include <neoneuron/application/NeoneuronApplication.h>
#include <neoneuron/ui/style/MaterialSymbols.h>

namespace
{

    const ImGuiID SPEED_POPUP_ID = ImHashStr("##combo_popup");

    constexpr size_t SPEEDS_AMOUNT = 8;
    constexpr size_t SPEED_DEFAULT = 3;

    constexpr std::array<const char*, SPEEDS_AMOUNT> SPEED_TITLES{
        ICON_MS_SPEED_0_2X, ICON_MS_SPEED_0_5X, ICON_MS_SPEED_0_7X, ICON_MS_1X_MOBILEDATA,
        ICON_MS_SPEED_1_2X, ICON_MS_SPEED_1_5X, ICON_MS_SPEED_1_7X, ICON_MS_SPEED_2X,
    };

    constexpr std::array<float, SPEEDS_AMOUNT> SPEED_MULTIPLIER{
        0.2f, 0.5f, 0.7f, 1.0f, 1.2f, 1.5f, 1.7f, 2.0f,
    };

} // namespace

namespace neoneuron
{

    void Timeline::sample(SampledEntry& entry, const mindset::EventSequence<std::monostate>* sampler,
                          std::chrono::nanoseconds step)
    {
        auto start = sampler->getStartTime();
        auto end = sampler->getEndTime();
        auto size = (end - start) / step;

        entry.times.reserve(size + 1);
        entry.samples.reserve(size + 1);

        entry.times.push_back(0);
        entry.samples.push_back(0.0f);

        for (size_t i = 0; i < size; i++) {
            auto from = start + step * i;
            auto to = from + step;

            std::ranges::subrange events = sampler->getRange(from, to);
            size_t amount = std::ranges::distance(events);

            auto seconds = std::chrono::duration_cast<std::chrono::duration<double>>(from);

            entry.times.push_back(seconds.count());
            entry.samples.push_back(static_cast<double>(amount));
        }

        entry.duration = sampler->getEndTime();
        entry.version = sampler->getVersion();
    }

    void Timeline::sample(SampledEntry& entry, const mindset::TimeGrid<double>* sampler)
    {
        entry.times.reserve(sampler->getTimestepsAmount());
        entry.samples.reserve(sampler->getTimestepsAmount());

        auto time = std::chrono::nanoseconds(0);
        for (auto data : sampler->getData()) {
            double sum = std::accumulate(data.begin(), data.end(), 0.0);
            entry.samples.push_back(sum);
            entry.times.push_back(static_cast<double>(time.count()));
            time += sampler->getDelta();
        }

        entry.duration = sampler->getDelta() * (sampler->getTimestepsAmount() - 1);
        entry.version = sampler->getVersion();
    }

    void Timeline::checkTimelines()
    {
        bool recalculateTimes = false;
        for (auto& sampled : _samples | std::views::values) {
            sampled.deleteFlag = true;
            for (auto& [name, entry] : sampled.entries) {
                sampled.deleteFlag = true;
            }
        }

        for (auto& aware : _timeAwares) {
            for (const auto& entry : aware->getEventSequences()) {
                auto activity = _application->getRepository().getActivity(entry.activityId);
                if (!activity) {
                    continue;
                }
                auto& sampled = _samples[entry.activityId];
                if (sampled.version != activity.value()->getVersion()) {
                    sampled.entries.clear();
                    sampled.version = activity.value()->getVersion();
                    recalculateTimes = true;
                }
                sampled.deleteFlag = false;

                auto& sampledEntry = sampled.entries[entry.name];
                if (sampledEntry.version != entry.entry->getVersion()) {
                    // Resample
                    sample(sampledEntry, entry.entry, entry.entry->getDuration() / 100);
                    recalculateTimes = true;
                }
                sampledEntry.deleteFlag = false;
            }

            for (const auto& entry : aware->getTimeGrids()) {
                auto activity = _application->getRepository().getActivity(entry.activityId);
                if (!activity) {
                    continue;
                }
                auto& sampled = _samples[entry.activityId];
                if (sampled.version != activity.value()->getVersion()) {
                    sampled.entries.clear();
                    sampled.version = activity.value()->getVersion();
                    recalculateTimes = true;
                }
                sampled.deleteFlag = false;

                auto& sampledEntry = sampled.entries[entry.name];
                if (sampledEntry.version != entry.entry->getVersion()) {
                    // Resample
                    sample(sampledEntry, entry.entry);
                    recalculateTimes = true;
                }
                sampledEntry.deleteFlag = false;
            }
        }

        if (std::erase_if(_samples, [&](const auto& p) { return p.second.deleteFlag; })) {
            recalculateTimes = true;
        }
        for (auto& entry : _samples | std::views::values) {
            if (std::erase_if(entry.entries, [&](const auto& p) { return p.second.deleteFlag; })) {
                recalculateTimes = true;
            }
        }

        if (recalculateTimes) {
            recalculateTimelineSize();
            _fitNextFrame = true;
        }
    }

    void Timeline::recalculateTimelineSize()
    {
        _duration = std::chrono::nanoseconds(0);

        if (_timeAwares.empty()) {
            return;
        }

        for (auto& activity : _samples | std::views::values) {
            for (auto& entry : activity.entries | std::views::values) {
                if (_duration < entry.duration) {
                    _duration = entry.duration;
                }
            }
        }
    }

    void Timeline::drawControlPanel()
    {
        auto& rows = ImGui::neon::BeginRowLayout("timeline_buttons");

        ImGui::PushFont(ImGui::GetFont(), 42.0f);

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetColorU32(ImGuiCol_FrameBgHovered));
        ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.5f, 0.5f));

        if (rows.stretchedButton(ICON_MS_REPLAY, 0.25f)) {
            changeTime(std::chrono::nanoseconds(0), TimeChangeType::JUMP);
            _playing = true;
        }

        if (rows.stretchedButton(_playing ? ICON_MS_PAUSE : ICON_MS_RESUME, 0.5f)) {
            _playing = !_playing;
        }

        bool popupOpen = ImGui::IsPopupOpen(SPEED_POPUP_ID, ImGuiPopupFlags_None);
        if (rows.stretchedButton(SPEED_TITLES[_selectedSpeed], 0.25f) && !popupOpen) {
            ImGui::OpenPopup(SPEED_POPUP_ID);
            popupOpen = true;
        }

        if (popupOpen) {
            auto bb = ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
            if (ImGui::BeginComboPopup(SPEED_POPUP_ID, bb, ImGuiComboFlags_None)) {
                for (size_t n = 0; n < SPEED_TITLES.size(); n++) {
                    bool selected = n == _selectedSpeed;
                    if (ImGui::Selectable(SPEED_TITLES[n], selected)) {
                        _selectedSpeed = n;
                    }
                    if (selected) {
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }
        }
        ImGui::PopStyleColor(3);
        ImGui::PopStyleVar();

        ImGui::PopFont();

        rows.end();
    }

    void Timeline::changeTime(std::chrono::nanoseconds newTime, TimeChangeType type)
    {
        for (auto aware : _timeAwares) {
            aware->onTimeChanged(_currentTime, newTime, type);
        }
        _currentTime = newTime;
    }

    Timeline::Timeline(NeoneuronApplication* application) :
        _application(application),
        _duration(0),
        _currentTime(0),
        _playing(false),
        _selectedSpeed(SPEED_DEFAULT),
        _fitNextFrame(false)
    {
    }

    void Timeline::addTimeAware(std::shared_ptr<TimeAware> timeAware)
    {
        _timeAwares.insert(std::move(timeAware));
    }

    void Timeline::removeTimeAware(std::shared_ptr<TimeAware> timeAware)
    {
        _timeAwares.erase(std::move(timeAware));
    }

    void Timeline::onUpdate(float deltaTime)
    {
        if (!_playing) {
            return;
        }

        auto delta = std::chrono::duration<float>(deltaTime * SPEED_MULTIPLIER[_selectedSpeed]);

        auto now = _currentTime + std::chrono::duration_cast<std::chrono::nanoseconds>(delta);

        if (_duration.count() > 0) {
            now %= _duration;
            changeTime(now, TimeChangeType::JUMP);
        } else {
            changeTime(now, TimeChangeType::FLOW);
        }
    }

    void Timeline::onPreDraw()
    {
        ImPlot::GetInputMap().Pan = ImGuiMouseButton_Middle;

        checkTimelines();

        if (_fitNextFrame) {
            ImPlot::SetNextAxesToFit();
            _fitNextFrame = false;
        }

        ImPlot::ShowDemoWindow();
        if (ImGui::Begin("Timeline")) {
            auto& layout = ImGui::neon::BeginColumnLayout("timeline_layout");

            ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.25f);
            if (ImPlot::BeginPlot("Timeline", ImVec2(-1, layout.popStretchedSize()), ImPlotFlags_NoTitle)) {
                ImPlot::SetupAxes("Time (s)", "Sum", ImPlotAxisFlags_None, ImPlotAxisFlags_AutoFit);

                if (ImPlot::IsPlotHovered() && ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
                    ImPlotPoint pos = ImPlot::GetPlotMousePos();
                    auto now =
                        std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::duration<double>{pos.x});
                    changeTime(now, TimeChangeType::JUMP);
                }

                auto currentTime = std::chrono::duration_cast<std::chrono::duration<double>>(_currentTime);

                float x1 = ImPlot::PlotToPixels(ImPlotPoint(static_cast<float>(currentTime.count() - 1), 0.0f)).x;
                float x2 = ImPlot::PlotToPixels(ImPlotPoint(static_cast<float>(currentTime.count()), 0.0f)).x;
                ImPlot::PushPlotClipRect();
                ImPlot::GetPlotDrawList()->AddRectFilledMultiColor({x1, 0}, {x2, 10000.0f}, 0x0000FF00, 0x5500FF00,
                                                                   0x5500FF00, 0x0000FF00);
                ImPlot::GetPlotDrawList()->AddLine({x2, 0}, {x2, 10000.0f}, 0xFF00FF00, 2.0f);
                ImPlot::PopPlotClipRect();
                for (auto& [gid, activity] : _samples) {
                    for (auto& [name, sampled] : activity.entries) {
                        auto plotName = std::format("{}##{}_{}", name, gid.datasetId, gid.internalId);
                        ImPlot::PlotShaded(plotName.c_str(), sampled.times.data(), sampled.samples.data(),
                                           sampled.samples.size(), 0);
                    }
                }

                ImPlot::EndPlot();
            }
            ImPlot::PopStyleVar();

            layout.next(true);

            drawControlPanel();

            layout.next(false);
            layout.end();
        }
        ImGui::End();
    }

} // namespace neoneuron
