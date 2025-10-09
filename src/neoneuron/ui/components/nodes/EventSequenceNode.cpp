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

#include "EventSequenceNode.h"

#include <mindset/EventSequence.h>

namespace neoneuron
{

    bool EventSequenceId::operator==(const EventSequenceId& rhs) const
    {
        return datasetId == rhs.datasetId && activityId == rhs.activityId && eventSequenceId == rhs.eventSequenceId;
    }

    bool EventSequenceId::operator!=(const EventSequenceId& rhs) const
    {
        return datasetId != rhs.datasetId || activityId != rhs.activityId || eventSequenceId != rhs.eventSequenceId;
    }

    void EventSequenceNode::sendEventSequence(EventSequenceId id)
    {
        sendOutput("Sequence", id);

        auto* activity = _application->getRepository().getActivity({id.datasetId, id.activityId}).value();
        auto* sequence = activity->getPropertyPtr<mindset::EventSequence<std::monostate>>(id.eventSequenceId).value();
        _eventSequenceVersion = sequence->getVersion();
        _activityVersion = activity->getVersion();
    }

    EventSequenceNode::EventSequenceNode(NeoneuronApplication* application) :
        Node("EventSequence"),
        _application(application),
        _selectedEventSequence(0)
    {
        defineOutput<EventSequenceId>("Sequence");
    }

    void EventSequenceNode::renderBody()
    {
        constexpr size_t MAX_EVENT_SEQUENCES = 1024;
        std::string names[MAX_EVENT_SEQUENCES];
        const char* namesC[MAX_EVENT_SEQUENCES];
        EventSequenceId ids[MAX_EVENT_SEQUENCES];
        const mindset::EventSequence<std::monostate>* selected = nullptr;
        const mindset::Activity* selectedActivity = nullptr;

        int i = 0;
        for (auto [datasetUID, dataset] : _application->getRepository().getDatasets()) {
            auto& prop = dataset->getDataset().getProperties();
            for (auto activity : dataset->getDataset().getActivities()) {
                for (auto& [uid, any] : activity->getProperties()) {
                    if (i >= MAX_EVENT_SEQUENCES) {
                        break;
                    }

                    auto* sequence = std::any_cast<mindset::EventSequence<std::monostate>>(&any);
                    if (sequence == nullptr) {
                        continue;
                    }

                    EventSequenceId id = {datasetUID, activity->getUID(), uid};

                    auto name = prop.getPropertyName(uid).value_or("Unknown");

                    names[i] = std::format("{} - {} - {} ", dataset->getName(), uid, name);
                    namesC[i] = names[i].c_str();
                    ids[i] = id;

                    if (!_selectedEventSequenceId.has_value()) {
                        _selectedEventSequenceId = id;
                        _selectedEventSequence = i;
                        selected = sequence;
                        selectedActivity = activity;
                        sendEventSequence(id);
                    } else if (_selectedEventSequenceId == id) {
                        _selectedEventSequence = i;
                        selected = sequence;
                        selectedActivity = activity;
                    }

                    ++i;
                }
            }
        }

        if (selected == nullptr && i > 0) {
            // EventSequence has been deleted
            _selectedEventSequence = 0;
            _selectedEventSequenceId = ids[0];
            sendEventSequence(ids[0]);
        } else if (i == 0 && _selectedEventSequenceId.has_value()) {
            _selectedEventSequenceId = {};
            _selectedEventSequence = 0;
            sendOutput("Sequence", std::any());
        } else if (selected != nullptr && (selected->getVersion() != _eventSequenceVersion ||
                                           selectedActivity->getVersion() != _activityVersion)) {
            sendEventSequence(*_selectedEventSequenceId);
        }

        ImGui::SetNextItemWidth(std::min(ImGui::GetContentRegionAvail().x, 200.0f));
        if (ImGui::Combo("##eventSequence", &_selectedEventSequence, namesC, i)) {
            _selectedEventSequenceId = ids[_selectedEventSequence];
            sendEventSequence(*_selectedEventSequenceId);
        }
    }

    NodeFactory EventSequenceNode::createFactory()
    {
        return NodeFactory("Event sequence", [](ImBlueprint::Editor& editor, auto* app) {
            return editor.addNode<EventSequenceNode>(app);
        });
    }
} // namespace neoneuron
